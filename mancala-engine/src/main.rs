const NUM_HOLES: usize = 6;

#[derive(Clone, PartialEq)]
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

fn make_move(board: &mut Board, move_: i16) {
    let mut idx: i16 = (((move_ >> 1) & 0x7)).into();

    board.pebbles[board.turn as usize][idx as usize] = 0;

    let mut num_pebbles = move_ >> 4;
    let mut side_placing_on = board.turn;

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

        if idx != NUM_HOLES.try_into().unwrap() {
            board.pebbles[side_placing_on as usize][idx as usize] += 1;
        }
        else {
            if side_placing_on == board.turn {
                board.scores[board.turn as usize] += 1;
            }
            else {
                // On opponent's store, we aren't actually placing
                // on anything this iteration.
               num_pebbles += 1;
            }

            idx = -1;
            side_placing_on = !side_placing_on;
        }
    }
    // Checks for captures
    if side_placing_on == board.turn && board.pebbles[board.turn as usize][idx as usize] == 1 {
        let num_pebbles_captured = board.pebbles[!board.turn as usize][5-idx as usize];
        board.pebbles[!board.turn as usize][5-idx as usize] = 0;
        board.pebbles[board.turn as usize][idx as usize] = 0;

        board.capture_stack.push(num_pebbles_captured);
        board.scores[board.turn as usize] += num_pebbles_captured+1
    }
    board.turn = if idx == -1 { board.turn } else { !board.turn };
}

fn unmake_move(board: &mut Board, move_: i16) {
    board.turn = (move_ & 1) != 0;

    let mut num_pebbles = move_ >> 4;
    let mut side_placing_on = board.turn;
    let mut idx: i16 = (((move_ >> 1) & 0x7)) as i16;

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

        if idx != NUM_HOLES.try_into().unwrap() { 
            board.pebbles[side_placing_on as usize][idx as usize] -= 1;
        }
        else {
            if side_placing_on == board.turn {
                board.scores[board.turn as usize] -= 1;
            }
            else { 
                // Same story as in make_move().
                num_pebbles += 1;
            }

            idx = -1;
            side_placing_on = !side_placing_on;
        }
    }
    board.pebbles[board.turn as usize][((move_ >> 1) & 0x7) as usize] = (move_ >> 4).into();

    // Undoing a capture
    if side_placing_on == board.turn && board.pebbles[board.turn as usize][idx as usize] < 0 {
        let mut capture_amount = board.capture_stack[board.capture_stack.len()-1];

        board.scores[board.turn as usize] -= capture_amount+1; 
        board.pebbles[board.turn as usize][idx as usize] = 0;

        // This is needed to ensure that we restore the right number of 
        // pebbles to the opponent. If we have looped around once when making 
        // the move, then the opponent will have one more pebble in the hole
        // being captured than they had before the move was made.
        if ((move_ >> 1) & 0x7) + (move_ >> 4) >= (2*NUM_HOLES + 1).try_into().unwrap() {
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
    fn make_unmake_moves() {
        let mut boards:Vec<Board> = vec![
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
        let mut moves: Vec<i16> = vec! [
            0xaa,
            0x53,
            0x6b,
            0x14,
            0x51,
            0x36,
        ];

        let mut test_passed = true;
        test_passed &= test_make_unmake_prototype(&boards, &moves, "make");

        boards.reverse();
        moves.reverse();

        test_passed &= test_make_unmake_prototype(&boards, &moves, "unmake");

        if !test_passed {
            panic!();
        }
    }

    fn test_make_unmake_prototype(boards: &Vec<Board>, moves: &Vec<i16>, move_type: &str) -> bool {
        let mut board = boards[0].clone();
        let perform_operation: fn(&mut Board, i16) = if move_type == "make" { make_move } else { unmake_move };

        for i in 0..boards.len()-1 {
            perform_operation(&mut board, moves[i]);
            
            if board != boards[i+1] {
                println!("failed {move_type}! 0x{:x}", moves[i]);
                print_board(&board);
                print_board(&boards[i+1]);
                print_board(&boards[i]);

                return false;
            }
        }
        return true;
    }
}

fn main() {

}
