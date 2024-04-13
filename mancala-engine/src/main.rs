const NUM_HOLES: usize = 6;

struct Board {
    pebbles: [[u16; NUM_HOLES]; 2],
    scores: [u16; 2],
    turn: bool,
    capture_stack: Vec<u16>,
}

fn print_board(board: &Board) {
    for i in 0..NUM_HOLES {
        println!("{:2} {:2}", board.pebbles[0][i], board.pebbles[1][NUM_HOLES-i-1]);
    }
    println!("scores: {}, {}", board.scores[0], board.scores[1]);
    println!("turn: {}", board.turn);
}

/* Moves are stored in this format:
* bit 0: turn before the move was played
* bits 1..4: index of pebbles being moved
* bits 4..: number of pebbles being moved
*/

fn make_move(board: &mut Board, mmove: u16) {
    let mut idx: i8 = (((mmove >> 1) & 0x7)) as i8;

    board.pebbles[usize::from(board.turn)][idx as usize] = 0;

    let mut num_pebbles = mmove >> 4;
    let mut side_placing_on = board.turn;

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

        // println!("pebbles: {num_pebbles}, idx: {idx}");

        if idx == NUM_HOLES.try_into().unwrap() { // Placing on someone's store
            if side_placing_on == board.turn {
                board.scores[board.turn as usize] += 1;
            }
            else {
                // Num pebbles was decremented assuming that a pebble was
                // going to be placed. Because it's on the other oppenent's
                // store, a pebble is not placed and the pebble is restored.
                num_pebbles += 1;
            }

            idx = -1;
            // println!("this deviousness");
            side_placing_on = !side_placing_on;
        }
        else {
            board.pebbles[usize::from(side_placing_on)][idx as usize] += 1;
        }
    }
    // Checks for free turn and captures
    if side_placing_on == board.turn && board.pebbles[usize::from(board.turn)][idx as usize] == 1 {
        let num_pebbles_captured = board.pebbles[usize::from(!board.turn)][5-idx as usize] + 1;
        board.pebbles[usize::from(!board.turn)][5-idx as usize] = 0;
        board.pebbles[usize::from(board.turn)][idx as usize] = 0;

        board.capture_stack.push(num_pebbles_captured);
        board.scores[usize::from(board.turn)] += num_pebbles_captured
    }
    // println!("idx: {idx}, holes: {NUM_HOLES}");
    board.turn = if idx == -1 { board.turn } else { !board.turn };
}

fn unmake_move(board: &mut Board, mmove: u16) {
    board.turn = (mmove & 1) != 0;

    let mut num_pebbles = mmove >> 4;
    let mut side_placing_on = board.turn;
    let mut idx: usize = (((mmove >> 1) & 0x7)+1).into();

    while num_pebbles > 0 {
        num_pebbles -= 1;

        if idx == NUM_HOLES { // Placing on someone's store
            if side_placing_on == board.turn {
                board.scores[usize::from(board.turn)] -= 1;
            }
            else {
                num_pebbles += 1;
            }

            idx = 0;
            side_placing_on = !side_placing_on;
        }
        else {
            board.pebbles[usize::from(side_placing_on)][idx] -= 1;
            idx += 1;
        }
    }

    board.pebbles[usize::from(board.turn)][usize::from((mmove & 0xe) >> 1)] = mmove >> 4;
}

fn main() {
    let mut board = Board {
        pebbles: [[4, 4, 0, 4, 0, 10], [4, 4, 4, 0, 4, 4]],
        scores: [0, 0],
        turn: false,
        capture_stack: Vec::with_capacity(NUM_HOLES*2),
    };
    print_board(&board);
    let mut new_move = 0xaa;
    // let mut new_move = 0x40;
    make_move(&mut board, new_move);
    print_board(&board);
    new_move = 0x53;
    make_move(&mut board, new_move);
    print_board(&board);
}
