import gevent.monkey
gevent.monkey.patch_all();

from flask import Flask, render_template
from jinja2 import Environment, FileSystemLoader
from flask_socketio import SocketIO
import json
import sqlite3
import pandas as pd
import re

# Gui Architecture
# - Follows the MVC architecture.
# - Flask serves up an html/JS GUI.
# - The model application (sim, robot, etc.) is a separate python program.
# - The GUI and model connect to Flask via websockets.
# - Data is exchanged between the GUI and model using an sqlite3 database.
# - The shared data is a single JS object/python dictionary stored in the database.
# - GUI dataflow:
#  > Initiates write each time user input is received.
#  > Initiates read on a regular refresh rate for the view.
# - Model dataflow:
#  > Initiates read and write on a suitable timeframe for the application.
#  > Responsible for initial population of the database fields.


# ---- Routines ----

def ResetDatabase():
    
    try:
        c.execute("drop table guiData;")
    except sqlite3.Error as err:
        pass
    
    c.execute("create table guiData (ind integer);")

    conn.commit();

    
    

# ---- Setup ----


# Configure flask socketio
app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketIo = SocketIO(app)

# Reset database
conn = sqlite3.connect("gui-data.db")
c = conn.cursor();

ResetDatabase();

print("Reset database.");
print(pd.read_sql_query("SELECT * FROM guiData", conn));




# ---- Flask Misc Routines ----
@app.route('/') 
def index():
    
    return render_template('index.html'); 


# ---- Model Interaction Routines ----

@socketIo.on('init request')
def DatabaseModelInitEvent(jsonData):

    print('-- Model Initializing Database --');
    
    ResetDatabase();
    
    colReq = json.loads(jsonData);

    print(" Columns: ",colReq);
    
    colNames = re.split("[']", "%s" % colReq.keys())[1:-1:2];
    colTypes = re.split("[']", "%s" % colReq.values())[1:-1:2];

    for iCol in range(len(colNames)):    
        c.execute("alter table guiData add %s %s;" % ( colNames[iCol], colTypes[iCol] ));

    c.execute("insert into guiData (ind) values (0);");
    
    conn.commit();
           
    print(pd.read_sql_query("SELECT * FROM guiData", conn));
 
    

@socketIo.on('update request')
def DatabaseModelUpdateEvent(jsonData):
    
    newEntry = json.loads(jsonData);

    print('-- Model Updating Database --');
    print(' Data: ',newEntry);
    
    entryNames = re.split("[']", "%s" % newEntry.keys())[1:-1:2];
    entryValues = re.split("[, |\[|\]]", "%s" % newEntry.values())[1:-1:2];
  
    updateStr = "";
    for iCol in range(len(entryNames)):
        
        updateStr += "%s=%s," % ( entryNames[iCol], entryValues[iCol] );
        
    updateStr = updateStr[0:-1];
        
    c.execute("update guiData set %s where ind=0;" % updateStr );
        
    print(pd.read_sql_query("SELECT * FROM guiData", conn));
      
    conn.commit();
    
    
    
    
@socketIo.on('read request')
def DatabaseModelReadEvent(jsonData):
    

    print('-- Model Read Database --');

    reqNames = json.loads(jsonData);
    
    print(' Requested: ', reqNames );
        
    reply = {};
    for name in reqNames:
        try:            
            c.execute("select %s from guiData where ind=0;" % ( name ));
        except sqlite3.Error as err:
            return;
            
        reply[name] = c.fetchone()[0];

    print(' Data Returned: ', reply );
               
    socketIo.emit('read reply',json.dumps(reply));
        
        

if __name__ == '__main__':
    
    app.config['TEMPLATES_AUTO_RELOAD'] = True
    socketIo.run(app, debug=True)


