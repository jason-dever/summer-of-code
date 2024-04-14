const NUM_HOLES: usize = 6;

#[derive(Clone, PartialEq, Eq)]
struct Board {
    pebbles: [[i16; NUM_HOLES]; 2],
    scores: [i16; 2],
    turn: bool,
    capture_stack: Vec<i16>,
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

        board.capture_stack.push(num_pebbles_captured);
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
        let mut capture_amount = board.capture_stack[board.capture_stack.len()-1];

        board.scores[board.turn as usize] -= capture_amount+1; 
        // println!("undoing capture: idx is {idx}");
        board.pebbles[board.turn as usize][idx as usize] = 0;

        if ((mmove >> 1) & 0x7) + (mmove >> 4) >= 13 {
            capture_amount -= 1;
        }

        board.pebbles[!board.turn as usize][5-idx as usize] = capture_amount;
        board.capture_stack.pop();
    }

}

fn print_board(board: &Board) {
    for i in 0..NUM_HOLES {
        println!("{:2} {:2}", board.pebbles[0][i], board.pebbles[1][NUM_HOLES-i-1]);
    }
    println!("scores: {}, {}", board.scores[0], board.scores[1]);
    println!("turn: {}", board.turn);
    print!("[ ");
    for val in &board.capture_stack {
        print!("{val}, ");
    }
    println!("]\n");
}

#[cfg(test)]
mod tests {
    use crate::*;


    #[test]
    fn make_moves() {
        let boards:Vec<Board> = vec![
            Board { 
                pebbles: [[4, 4, 0, 1, 0, 10], [4, 4, 4, 0, 4, 4]],
                scores: [0, 0],
                turn: false,
                capture_stack: vec![],
            }, 
            Board {
                pebbles: [[5, 5, 0, 1, 0, 0], [5, 5, 5, 0, 5, 5]],
                scores: [3, 0],
                turn: true,
                capture_stack: vec![ 1 ],
            },
            Board {
                pebbles: [[5, 5, 0, 1, 0, 0], [5, 0, 6, 1, 6, 6]],
                scores: [3, 1],
                turn: true,
                capture_stack: vec![ 1 ],
            },
            Board {
                pebbles: [[6, 6, 1, 2, 1, 0], [5, 0, 6, 1, 6, 0]],
                scores: [3, 2],
                turn: false,
                capture_stack: vec![ 1 ],
            },
            Board {
                pebbles: [[6, 6, 0, 3, 1, 0], [5, 0, 6, 1, 6, 0]],
                scores: [3, 2],
                turn: true,
                capture_stack: vec![ 1 ],
            },
            Board {
                pebbles: [[0, 6, 0, 3, 1, 0], [0, 1, 7, 2, 7, 0]],
                scores: [3, 9],
                turn: false,
                capture_stack: vec![ 1, 6 ],
            },
            Board {
                pebbles: [[0, 6, 0, 0, 2, 1], [0, 1, 7, 2, 7, 0]],
                scores: [4, 9],
                turn: false,
                capture_stack: vec![ 1, 6 ],
            },
        ];
        let moves: Vec<i16> = vec! [
            0xaa,
            0x53,
            0x6b,
            0x14,
            0x51,
            0x36,
        ];

        let mut board = boards[0].clone();
        for i in 0..boards.len()-1 {
            make_move(&mut board, moves[i]);
            
            if board != boards[i+1] {
                println!("failed make!");
                print_board(&board);
                print_board(&boards[i+1]);
                print_board(&boards[i]);

                panic!();
            }
        }
    }

    #[test]
    fn unmake_moves() {
        let boards:Vec<Board> = vec![
            Board {
                pebbles: [[0, 6, 0, 0, 2, 1], [0, 1, 7, 2, 7, 0]],
                scores: [4, 9],
                turn: false,
                capture_stack: vec![ 1, 6 ],
            },
            Board {
                pebbles: [[0, 6, 0, 3, 1, 0], [0, 1, 7, 2, 7, 0]],
                scores: [3, 9],
                turn: false,
                capture_stack: vec![ 1, 6 ],
            },
            Board {
                pebbles: [[6, 6, 0, 3, 1, 0], [5, 0, 6, 1, 6, 0]],
                scores: [3, 2],
                turn: true,
                capture_stack: vec![ 1 ],
            },
            Board {
                pebbles: [[6, 6, 1, 2, 1, 0], [5, 0, 6, 1, 6, 0]],
                scores: [3, 2],
                turn: false,
                capture_stack: vec![ 1 ],
            },
            Board {
                pebbles: [[5, 5, 0, 1, 0, 0], [5, 0, 6, 1, 6, 6]],
                scores: [3, 1],
                turn: true,
                capture_stack: vec![ 1 ],
            },
            Board {
                pebbles: [[5, 5, 0, 1, 0, 0], [5, 5, 5, 0, 5, 5]],
                scores: [3, 0],
                turn: true,
                capture_stack: vec![ 1 ],
            },
            Board { 
                pebbles: [[4, 4, 0, 1, 0, 10], [4, 4, 4, 0, 4, 4]],
                scores: [0, 0],
                turn: false,
                capture_stack: vec![],
            }, 
        ];
        let moves: Vec<i16> = vec! [
            0x36,
            0x51,
            0x14,
            0x6b,
            0x53,
            0xaa,
        ];

        let mut board = boards[0].clone();
        for i in 0..boards.len()-1 {
            unmake_move(&mut board, moves[i]);
            
            if board != boards[i+1] {
                println!("failed unmake! 0x{:x}", moves[i]);
                print_board(&board);
                print_board(&boards[i+1]);
                print_board(&boards[i]);

                panic!();
            }
        }
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
