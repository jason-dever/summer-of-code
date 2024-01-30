import sys, pygame, time, random

random.seed()

pygame.init()
pygame.display.set_caption("pong game for emmy...")

size = width, height = 1600, 1200
screen = pygame.display.set_mode(size)

GREY = 25,25,25
WHITE = 255,255,255

left_paddle_speed = 0
right_paddle_speed = 0
paddle_move_speed = 13
ball_speed = [13,13]

ball = pygame.Rect(width/2, height/2, 50, 50)

left_paddle = pygame.Rect(30, (height/2)-125, 60, 250)
right_paddle = pygame.Rect(width-100, (height/2)-125, 60, 250)

clock = pygame.time.Clock()

left_points = 0
right_points = 0

def checkEvents():
    global right_paddle_speed
    global left_paddle_speed

    for event in pygame.event.get():
        if event.type == pygame.QUIT: 
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            key = event.key
            match key:
                case pygame.K_UP: 
                    right_paddle_speed = -paddle_move_speed; break
                case pygame.K_DOWN: 
                    right_paddle_speed = paddle_move_speed; break
                case pygame.K_COMMA: 
                    left_paddle_speed = -paddle_move_speed; break
                case pygame.K_o: 
                    left_paddle_speed = paddle_move_speed; break
        elif event.type == pygame.KEYUP:
            key = event.key
            if key == pygame.K_UP or key == pygame.K_DOWN:
                right_paddle_speed = 0
            if key == pygame.K_COMMA or key == pygame.K_o:
                left_paddle_speed = 0

def drawBoard():
    screen.fill(GREY)

    pygame.draw.rect(screen, WHITE, ball)    
    pygame.draw.rect(screen, WHITE, left_paddle)
    pygame.draw.rect(screen, WHITE, right_paddle)

    pygame.display.flip()

def moveObjects():
    global ball
    global left_paddle
    global right_paddle
    global ball_speed

    global right_points
    global left_points

    ball = ball.move(ball_speed)

    if ball.left < 0:
        right_points += 1
        print("Right scored! Left score: %s, Right score: %s" % (left_points, right_points))
        ball = pygame.Rect(width/2, height/2, 50, 50)
        ball_speed = [ball_speed[0]*random.choice((1, -1)), ball_speed[1]*random.choice((1, -1))]
        time.sleep(0.25)
    elif ball.right > width:
        left_points += 1
        print("Left scored! Left score: %s, Right score: %s" % (left_points, right_points))
        ball = pygame.Rect(width/2, height/2, 50, 50)
        ball_speed = [ball_speed[0]*random.choice((1, -1)), ball_speed[1]*random.choice((1, -1))]
        time.sleep(0.25)

    if ball.top < 0 or ball.bottom > height:
        ball_speed[1] *= -1

    # Paddle contact
    if ball.left < left_paddle.right and ball.top > left_paddle.top and ball.bottom < left_paddle.bottom:
        ball_speed[0] *= -1
    if ball.right > right_paddle.left and ball.top > right_paddle.top and ball.bottom < right_paddle.bottom:
        ball_speed[0] *= -1

    if left_paddle_speed < 0:
        if left_paddle.top > 0:
            left_paddle = left_paddle.move([0, left_paddle_speed])
    else:
        if left_paddle.bottom < height:
            left_paddle = left_paddle.move([0, left_paddle_speed])

    if right_paddle_speed < 0:
        if right_paddle.top > 0:
            right_paddle = right_paddle.move([0, right_paddle_speed])
    else:
        if right_paddle.bottom < height:
            right_paddle = right_paddle.move([0, right_paddle_speed])

while True:    
    checkEvents()
    moveObjects()
    drawBoard()
    clock.tick(60)