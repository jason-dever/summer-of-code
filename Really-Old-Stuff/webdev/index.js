var Direction = [1, 0];
var turned = false;

// Sleep function
const sleep = (ms) => new Promise(r => this.setTimeout(r, ms));

window.addEventListener("keydown", function (event) {
	// Keydown events: LEFT and RIGHT make the head move left or right, and UP and DOWN make the head move up or down. The turned variable ensures the snake can only turn once before moving.
    if (turned === false) {
		switch (event.code) {
			case "KeyA":
				if (Direction[0] === 0) {Direction = [-1, 0]; turned = true;}
				break;
			case "KeyD":
				if (Direction[0] === 0) {Direction = [1, 0]; turned = true;}
				break;
			case "KeyW":
				if (Direction[1] === 0) {Direction = [0, -1]; turned = true;}
				break;
			case "KeyS":
				if (Direction[1] === 0) {Direction = [0, 1]; turned = true;}
		}
	}
});

function foodInExcluded(food, excluded) {
    for (let x in excluded) {
        if (food[0] === excluded[x][0] && food[1] === excluded[x][1]) {
            return true;
        }
    }
    return false;
}

function moveSnake(Direction, SSL, food) {
    let tempPos = SSL[SSL.length-1];
    for (let x = SSL.length-1; x > 0; x--) { // Loops through the snake's tail and moves each segment's location forward to the location of the previous segment in the list
        SSL[x] = SSL[x-1];
    }

    SSL[0] = [SSL[0][0] + Direction[0], SSL[0][1] + Direction[1]]; // Moves head

    if (SSL[0][0] < 0 || SSL[0][0] > 15 || SSL[0][1] < 0 || SSL[0][1] > 15) { // Head is out of bounds
        return [SSL, true, food];
    }

    // Loops through SSL and checks if the head is touching any other segments of the snake
    for (let x = 1; x < SSL.length - 1; x ++) {
        if (SSL[0][0] === SSL[x][0] && SSL[0][1] === SSL[x][1]) {
            return [SSL, true, food];
        }
    }

    if (SSL[0][0] === food[0] && SSL[0][1] === food[1]) { // Head hit the food, extend snake length by one
        SSL.push(tempPos)
        food = getFood(SSL, food);
    }   
    return [SSL, false, food];
}

function getFood(excluded, food) {
    // Generates coordinates for food until its coordinates are not equal to the coordinates of any of the snake segments.
    while (true) { 
        food = [Math.round(Math.random() * 15), Math.round(Math.random() * 15)];
        if (foodInExcluded(food, excluded) === false) {
            return food;
        }
    } 
}

function drawSnake(SSL) {
    for (let x in SSL) {
        let coordinates = SSL[x];
        let segmentClass = "column".concat(coordinates[0].toString()).concat("row".concat(coordinates[1].toString()));
        let segmentDiv = document.getElementById(segmentClass);
        segmentDiv.style.backgroundColor = "blue";
    } 
}

function drawFood(food) {
    let foodClass = "column".concat(food[0].toString()).concat("row".concat(food[1].toString()));
    let foodDiv = document.getElementById(foodClass);
    foodDiv.style.backgroundColor = "red";
}

function drawBoard() {
    // Redraw empty board
    for (let row = 0; row < 16; row++) {
        for (let column = 0; column < 16; column++) {
            const sq = document.getElementById("column".concat(column.toString()).concat("row".concat(row.toString())));
            sq.style.backgroundColor = "lime";

            tempColumn = column; 
            if (row%2 === 0) {
                tempColumn += 1;
            }
            if (tempColumn%2 === 0) {
                sq.style.backgroundColor = "green";
            }
        }
    }
}

window.addEventListener("load", async function (){ // Code only runs when all of the HTML is loaded in

    const snakeDiv = document.getElementById("snake_game_container");

    // Draws board onto the page
    for (let row = 0; row < 16; row++) { // Makes 16 rows
        const row_e = document.createElement("div");
        row_e.classList.add("row_".concat(row.toString()));
        row_e.style.width = "960px";
        row_e.style.height = "60px";
        row_e.style.display = "flex";

        snakeDiv.appendChild(row_e);

        for (let column = 0; column < 16; column++) { // Creates 16 square div elements on each row
            const sq = document.createElement("div");
            sq.id = "column".concat(column.toString()).concat("row".concat(row.toString())); // Gives square an id with the convention rowXcolumnY
            sq.style.width = "60px";
            sq.style.height = "60px";
            sq.style.backgroundColor = "lime";
            
            // Alternates lime and green squares
            tempColumn = column;
            if (row%2 === 0) { 
                tempColumn += 1;
            }
            if (tempColumn % 2 === 0) {
                sq.style.backgroundColor = "green";
            }

            row_e.appendChild(sq); // Adds sq div element to the row
        }
    }

    // Initialize basic variables; turned is a boolean used to tell if the snake has turned that loop, and food stores the coordinates of the snake's food.
    let gameOver = false;
    let food = [10, 7];
    let SSL = [[7, 7], [6, 7], [5, 7], [4, 7]];
    var returnedArr;

    // Begin game loop
    while (true) {
        turned = false;
        if (gameOver) {
            // End game loop
            break;
        }
        drawBoard();
        drawSnake(SSL);
        drawFood(food);
        returnedArr = moveSnake(Direction, SSL, food);
        SSL = returnedArr[0]; 
        gameOver = returnedArr[1]; 
        food = returnedArr[2];
        await sleep(120);
    }
})