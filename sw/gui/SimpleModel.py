import socketio
import json
import time

# Model Program
# - The 'model' in MVC nomenclature.
# - Represents a simulation or robot interface.


sio = socketio.Client();
sio.connect("http://localhost:5000");

@sio.on('read reply')
def ServerReadResponseEvent(jsonData):
    
    guiDbaseData = json.loads(jsonData);
    
    print('Received: ',jsonData);

    if( guiDbaseData['doubleRequest'] == 1):
        
        updateData = { 'dataValue': guiDbaseData['dataValue']*2, 'doubleRequest': 0 };
        
        sio.emit('update request', json.dumps(updateData) );
    




if __name__ == '__main__':
    
    guiDbaseFields = { 'circleXPos': 'real', 'circleYPos': 'real', 'dataValue': 'real', 'doubleRequest': 'integer' }
    guiDbaseData = { 'circleXPos': 0.0, 'circleYPos': 0.0, 'dataValue': 0.0, 'doubleRequest': 0 }
    
    # It's the model's responsibility to initialize the server databse fields and populate with initial values.
    sio.emit('init request', json.dumps(guiDbaseFields) );
    sio.emit('update request', json.dumps(guiDbaseData) );
        
    while(1):
        
        
        # Do some kind of work...zzzz
        time.sleep(3);
        
        print('Woke!');
        
        
        # The purpose of this simple model is to check to see if the GUI user has requested a doubling
        # of the value entered into the text box.  This is determined by initiating a read of the database
        # maintained by the server.
        
        sio.emit('read request', json.dumps( list(guiDbaseData.keys()) ) );
        

        
    
