import pygame, sys, random
pygame.init()
pygame.display.set_caption("Snake")

def checkEvents(Direction, turned):

    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit # Call for events every loop, quit game on QUIT event
        elif event.type == pygame.KEYDOWN:
            if turned == False: # Ensures the direction of the head can only change once per frame
                if event.key == pygame.K_LEFT and Direction[0] == 0: # Keydown events: LEFT and RIGHT make the head move left or right, and UP and DOWN make the head move up or down
                    Direction = [-1,0]
                    turned = True
                elif event.key == pygame.K_RIGHT and Direction[0] == 0:
                    Direction = [1,0]
                    turned = True
                elif event.key == pygame.K_UP and Direction[1] == 0:
                    Direction = [0,-1]
                    turned = True
                elif event.key == pygame.K_DOWN and Direction[1] == 0:
                    Direction = [0,1]
                    turned = True

    return Direction, turned

def moveSnake(Direction, SSL, food):

    for x in range(len(SSL)-1, 0, -1): # Loops through the snake's tail and moves each segment's location forward to the location of the previous segment in the list
        SSL[x] = SSL[x-1].copy()

    SSL[0] = [SSL[0][0] + Direction[0], SSL[0][1] + Direction[1]] # Moves head

    if SSL[0][0] > 31 or SSL[0][1] > 31 or SSL [0][0] < 0 or SSL[0][1] < 0: # Check if the head has gone into the border
        return SSL, True, food

    # Loops through SSL and checks if the head is touching any other segments of the snake.
    for x in range(1, len(SSL) - 1):
        if SSL[0] == SSL[x]:
            return SSL, True, food
        
    if SSL[0] == food: # Head hit the food, extend snake length by one
        difference = [SSL[len(SSL)-1][0]-SSL[len(SSL)-2][0], SSL[len(SSL)-1][1]-SSL[len(SSL)-2][1]]
        tempSegment = [SSL[len(SSL)-1][0] + difference[0], SSL[len(SSL)-1][1] + difference[1]]
        SSL.append(tempSegment)

        food = getFood(SSL,food)

    return SSL, False, food

def drawSnake(SSL, screen):
    for x in range(len(SSL)):
        segment = pygame.Rect(SSL[x][0]*32, SSL[x][1]*32, 32, 32)
        pygame.draw.rect(screen, white, segment)
    return screen

def getFood(excluded, food): 

    '''
    This function randomly generates coordinates for the snake's food and loops through the list of snake segments to check the coordinates generated along each segment of the snake. 
    If the food is occupying the same square as one of the segments, the function calls itself to generate a new set of coordinates.
    At the end of the function, it will return the coordinates up as many layers as it went down, generating food coordinates that are not touching any part of the snake.
    '''

    food = [random.randint(0,31,), random.randint(0,31)]
    for x in excluded:
        if food == x:
            food = getFood(excluded, food)
    return food

def drawFood(food, screen):
    foodRect = pygame.Rect(food[0]*32, food[1]*32, 32, 32)
    pygame.draw.rect(screen, white, foodRect)
    return screen

SSL = [[9,7],[8,7],[7,7],[6,7]] # Snake Segment Locations
Direction = [1,0]

# Initialize basic variables; size contains the width and height of the game window and the colours use rgb values
size = width, height = 1024,1024
grey = 50,50,50
white = 255,255,255
turned = False
gameOver = False
food = [20,15]

screen = pygame.display.set_mode(size)

# Begin game loop
while True:
    turned = False
    if gameOver == True:
        sys.exit()

    Direction, turned = checkEvents(Direction, turned)
    SSL, gameOver, food = moveSnake(Direction,SSL,food)
    screen.fill(grey)
    Direction, turned = checkEvents(Direction, turned)
    screen = drawSnake(SSL, screen)
    screen = drawFood(food, screen)
    Direction, turned = checkEvents(Direction, turned)
    pygame.display.flip()

    pygame.time.Clock().tick(12) # Locks framerate to maximum 12 fps