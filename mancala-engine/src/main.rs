const NUM_HOLES: usize = 6;

struct Board {
    pebbles: [[i16; NUM_HOLES]; 2],
    scores: [i16; 2],
    turn: bool,
    capture_stack: Vec<i16>,
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

fn make_move(board: &mut Board, mmove: i16) {
    let mut idx: i16 = (((mmove >> 1) & 0x7)) as i16;

    board.pebbles[usize::from(board.turn)][idx as usize] = 0;

    let mut num_pebbles = mmove >> 4;
    let mut side_placing_on = board.turn;

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

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
            side_placing_on = !side_placing_on;
        }
        else {
            board.pebbles[usize::from(side_placing_on)][idx as usize] += 1;
        }
    }
    // Checks for captures
    if side_placing_on == board.turn && board.pebbles[board.turn as usize][idx as usize] == 1 {
        let num_pebbles_captured = board.pebbles[usize::from(!board.turn)][5-idx as usize];
        board.pebbles[usize::from(!board.turn)][5-idx as usize] = 0;
        board.pebbles[usize::from(board.turn)][idx as usize] = 0;

        board.capture_stack.push(num_pebbles_captured << 3 | idx);
        board.scores[usize::from(board.turn)] += num_pebbles_captured+1
    }
    board.turn = if idx == -1 { board.turn } else { !board.turn };
}

fn unmake_move(board: &mut Board, mmove: i16) {
    board.turn = (mmove & 1) != 0;

    let mut num_pebbles = mmove >> 4;
    let mut side_placing_on = board.turn;
    let mut idx: i16 = (((mmove >> 1) & 0x7)) as i16;

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

        if idx == NUM_HOLES.try_into().unwrap() { // Placing on someone's store
            if side_placing_on == board.turn {
                board.scores[board.turn as usize] -= 1;
            }
            else {
                num_pebbles += 1;
            }

            idx = -1;
            side_placing_on = !side_placing_on;
        }
        else {
            board.pebbles[side_placing_on as usize][idx as usize] -= 1;
        }
        // println!("pebbles: {num_pebbles}, idx: {idx}");
    }
    board.pebbles[board.turn as usize][((mmove >> 1) & 0x7) as usize] = (mmove >> 4).into();

    // Undoing a capture
    // println!("{side_placing_on}, {}, {idx}, {}", board.turn, board.pebbles[board.turn as usize][idx as usize]);
    if side_placing_on == board.turn && board.pebbles[board.turn as usize][idx as usize] < 0 {
        let top = board.capture_stack[board.capture_stack.len()-1];
        // idx = top & 0x7;
        let capture_amount = top >> 3;

        board.scores[board.turn as usize] -= capture_amount+1; 
        // println!("undoing capture: idx is {idx}");
        board.pebbles[board.turn as usize][idx as usize] = 0;
        board.pebbles[!board.turn as usize][5-idx as usize] = 0;

        board.capture_stack.pop();
    }

}

fn main() {
    let mut board = Board {
        pebbles: [[4, 4, 0, 4, 0, 10], [4, 4, 4, 0, 4, 4]],
        scores: [0, 0],
        turn: false,
        capture_stack: Vec::with_capacity(NUM_HOLES*2),
    };
    print_board(&board);
    let old_move = 0xaa;
    make_move(&mut board, old_move);
    print_board(&board);
    let new_move = 0x53;
    make_move(&mut board, new_move);
    print_board(&board);
    unmake_move(&mut board, new_move);
    print_board(&board);
    unmake_move(&mut board, old_move);
    print_board(&board);
}
