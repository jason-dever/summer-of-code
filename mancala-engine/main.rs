const NUM_HOLES: i32 = 6;

struct Board {
    pebbles: [[u16; NUM_HOLES as usize]; 2],
    scores: [u16; 2],
    turn: bool,
}

fn print_board(board: &Board) {
    for i in 0..NUM_HOLES {
        println!("{} {}", board.pebbles[0][i as usize], board.pebbles[1][(NUM_HOLES-i-1) as usize]);
    }
    println!("scores: {}, {}", board.scores[0], board.scores[1]);
    println!("turn: {}", board.turn);
}

/* Moves are stored in this format:
* bit 0: turn before the move was played
* bits 1..3: index of pebbles being moved
* bits 3..: number of pebbles being moved
*/

fn make_move(board: &mut Board, mmove: u16) {
    let mut idx: i32 = ((mmove & 0xe) >> 1).into();
    board.pebbles[board.turn as usize][idx as usize] = 0;

    let mut num_pebbles = mmove >> 4;
    let mut side_placing_on = board.turn;
    let mut new_turn = !board.turn;

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

        if idx == NUM_HOLES { // Placing on someone's store
            if side_placing_on == board.turn {
                board.scores[board.turn as usize] += 1;

                if num_pebbles == 0 {
                    new_turn = board.turn;
                }
            }
            else {
                // Num pebbles was decremented assuming that a pebble was
                // going to be placed. Because it's on the other oppenent's
                // store, a pebble is not placed and the pebble is restored.
                num_pebbles += 1;
            }

            idx = -1; // idx will be incremented to 0 before being used.
            side_placing_on = !side_placing_on;
        }
        else {
            board.pebbles[side_placing_on as usize][idx as usize] += 1;
        }
    }
    board.turn = new_turn;
}

fn unmake_move(board: &mut Board, mmove: u16) {
    board.turn = (mmove & 1) != 0;

    let mut num_pebbles = mmove >> 4;
    let mut side_placing_on = board.turn;
    let mut idx: i32 = ((mmove & 0xe) >> 1).into();

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

        if idx == NUM_HOLES { // Placing on someone's store
            if side_placing_on == board.turn {
                board.scores[board.turn as usize] -= 1;
            }
            else {
                num_pebbles += 1;
            }

            idx = -1; // idx will be incremented to 0 before being used.
            side_placing_on = !side_placing_on;
        }
        else {
            board.pebbles[side_placing_on as usize][idx as usize] -= 1;
        }
    }

    board.pebbles[board.turn as usize][((mmove & 0xe) >> 1) as usize] = mmove >> 4;
}

fn main() {
    let mut board = Board {
        pebbles: [[4, 4, 4, 4, 4, 10], [4, 4, 4, 4, 4, 4]],
        scores: [0, 0],
        turn: false,
    };
    print_board(&board);
    let new_move: u16 = 0xaa;
    make_move(&mut board, new_move);
    print_board(&board);
    let move2: u16 = 0x53;
    make_move(&mut board, move2);
    print_board(&board);
    unmake_move(&mut board, move2);
    print_board(&board);
    unmake_move(&mut board, new_move);
    print_board(&board);
}
