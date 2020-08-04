/*
 * Gui Demo Jscript Graphics Objects & Functions
 */

/*
 *  ---- Canvas Functions ----
 */

// Draws the background of the canvas
function DrawCanvasBackground(){
    cx.fillStyle = "#001484";
    cx.fillRect(0,0,canvasX,canvasY);
}

// Blanks the canvas
function BlankCanvas(){
    cx.clearRect(0,0,canvasX,canvasY);
    DrawCanvasBackground();
}

// Simple circle drawing routine
function DrawCircle(xPos,yPos,radius){
    cx.strokeStyle = "white";
    cx.lineWidth = 2;
	
    cx.beginPath();
    cx.arc(xPos, yPos, radius, 0, 2 * Math.PI);
    cx.stroke();
	
}

function PrintText(posX,posY,textStr){
    cx.font = Math.floor(rowSz * 1.25) + "px Gill Sans";
    cx.fillStyle = "white";
    cx.fillText(textStr,posX,posY);
}



/*
 * ---- Canvas Graphics Objects ----
 */

// Canvas dimensions.
let canvasX = 500;
let canvasY = 500;

// Canvas division values.
// - The canvas is divided into 'rows' to allow for text placement.
// - The padding is only required on the edge of the canvas since the font size
//   is selected to have the correct inter-line spacing.

let rowSz = 50;
let padSz = 4;
let nRow = Math.floor( canvasY/(rowSz+padSz) );

// This is just a marker for the middle of the canvas
let midCol = Math.floor( canvasX/2 );


// The canvasObjects array contains one object for each item on the canvas.  Object contents:
// - Information required to draw the object.
// - A Draw() method that does the object.
// - An Update() method that updates the graphic object info based on changes to guiData.

canvasObjects = [];

/*
 * A circle that follows mouse clicks and can be reset by a button press.
 */

circle = { xPos: canvasX/2, yPos: canvasY/2, rad: rowSz/2 };

circle.Draw = function() {
    DrawCircle( this.xPos, this.yPos, this.rad );
}

circle.Update = function(guiData) {
    this.xPos = guiData.circleXPos;
    this.yPos = guiData.circleYPos;
}

canvasObjects.push(circle);

/*
 * Text that displays a value entered by the user that is doubled by the model.
 */

doubleValText = { xPos: padSz, yPos: rowSz, value: 0 };

doubleValText.Draw = function() {
    PrintText(this.xPos,this.yPos,""+this.value);
}

doubleValText.Update = function(guiData) {
    this.value = guiData.dataValue;
}

canvasObjects.push(doubleValText);



/*
 * ---- Controller/Model Graphics Object ----
 */

// This object is shared with the server and carries controller and model updates.

guiData = {
    circleXPos: canvasX/2,
    circleYPos: canvasY/2,
    dataValue: 1.0,
    doubleRequest: 0
};


/*
 * ---- Events ----
 */

let elemTextBox = document.getElementById("textInput");
let elemButton = document.getElementById("buttonInput");
let elemCanvas = document.getElementById("canvasViewCtrl");

// The context variable allows stuff to be drawn on the canvas
let cx = elemCanvas.getContext("2d");


// Records mouse clicks on the canvas.
elemCanvas.addEventListener("click", event => {

    data = { circleXPos: event.pageX, circleYPos: event.pageY };

    SendUpdateRequest(data);
    UpdateSharedData(data);
    RefreshCanvas();

    
});


// Pressing the button doubles the value displayed on the screen.
elemButton.addEventListener("click", event => {

    data = { dataValue: parseFloat(elemTextBox.value), doubleRequest: 1 };

    SendUpdateRequest(data);
    UpdateSharedData(data);
    RefreshCanvas();

});
