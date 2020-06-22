/*
 * Gui Demo Jscript Core Functions
 */


/*
 * ---- Graphics Routines ----
 */

function RefreshCanvas(){

    BlankCanvas();

    for(let canvasObject of canvasObjects){
	canvasObject.Draw();
    }
    
}

function UpdateGraphicsObjects(){

    for(let canvasObject of canvasObjects){
	canvasObject.Update(guiData);
    }

}

function UpdateSharedData(data){

    for(let key of Object.keys(data)){
	guiData[key] = data[key];
    }

    UpdateGraphicsObjects();

}

// Request an update of server data every 500ms
let refreshTimer = setTimeout( () => {

    SendReadRequest( Object.keys(guiData) );

},500);
    





/*
 * ---- Communications Routines ----
 */

var socket = io('http://localhost:5000');

// Called when the socket first conects
socket.on( 'connect', function() {

    // Do nothing here for now.

    
});


socket.on( 'read reply', function( msg ){
    
    readData = JSON.parse(msg);

    UpdateSharedData(readData);
    UpdateGraphicsObjects();
    RefreshCanvas();

});

function SendUpdateRequest(data){
    
    socket.emit('update request', JSON.stringify(data) );

}

function SendReadRequest(keys){

    socket.emit('read request', JSON.stringify(keys) );

}

/*
 * ---- Main Program ----
 */

RefreshCanvas();


/*cx.strokeStyle = "blue";
cx.strokeRect(5, 5, 50, 50);
cx.lineWidth = 5;
cx.strokeRect(135, 5, 50, 50);

cx.font = "10px Gill Sans";
//cx.font = "10px Fantasy";
cx.fillText("I can draw text, too!", 10, 50);
*/
