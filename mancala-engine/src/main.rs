const NUM_POCKETS: usize = 6;

#[derive(Clone, PartialEq)]
struct Board {
    pebbles: [[i16; NUM_POCKETS]; 2],
    scores: [i16; 2],
    turn: bool,
}

fn make_move(board: &mut Board, mut idx: i16) {
    let mut num_pebbles = board.pebbles[board.turn as usize][idx as usize];
    let mut side_placing_on = board.turn;

    board.pebbles[board.turn as usize][idx as usize] = 0;

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

        if idx != NUM_POCKETS.try_into().unwrap() {
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

        board.scores[board.turn as usize] += num_pebbles_captured+1
    }
    board.turn = if idx == -1 { board.turn } else { !board.turn };
}

fn print_board(board: &Board) {
    println!("-------------------");
    println!("|                 |");
    println!("|       {:2}        |", board.scores[1]);
    println!("|                 |");
    println!("-------------------");

    for i in 0..NUM_POCKETS {
        println!("|        |        |");
        println!("|  {:2}    |  {:2}    |", board.pebbles[0][i], board.pebbles[1][NUM_POCKETS-i-1]);
        println!("|        |        |");
        println!("-------------------");
    }

    println!("|                 |");
    println!("|       {:2}        |", board.scores[0]);
    println!("|                 |");
    println!("-------------------");
    println!("turn: {}", board.turn);
}

fn is_gameover(board: &Board) -> bool {
    for row in board.pebbles {
        let mut all_counts_zero = true;
        for count in row {
            all_counts_zero &= count == 0;
        }
        if all_counts_zero {
            return true;
        }
    }
    return false;
}

fn perft(board: &mut Board, depth: i32) -> u64 {
    if depth == 0 || is_gameover(board) {
        return 1;
    }

    let mut num_nodes = 0;
    for i in 0..NUM_POCKETS {
        let num_pebbles = board.pebbles[board.turn as usize][i];

        if num_pebbles != 0 {
            let before = board.clone();
            make_move(board, i as i16);
            num_nodes += perft(board, depth-1);
            *board = before;
        }
    }
    return num_nodes;
}
#[cfg(test)]
mod tests {
    use crate::*;

    #[test]
    fn make_moves() {
        let boards = vec![
            Board { 
                pebbles: [[4, 4, 0, 1, 0, 10], [4, 4, 4, 0, 4, 4]],
                scores: [0, 0],
                turn: false,
            }, 
            Board {
                pebbles: [[5, 5, 0, 1, 0, 0], [5, 5, 5, 0, 5, 5]],
                scores: [3, 0],
                turn: true,
            },
            Board {
                pebbles: [[5, 5, 0, 1, 0, 0], [5, 0, 6, 1, 6, 6]],
                scores: [3, 1],
                turn: true,
            },
            Board {
                pebbles: [[6, 6, 1, 2, 1, 0], [5, 0, 6, 1, 6, 0]],
                scores: [3, 2],
                turn: false,
            },
            Board {
                pebbles: [[6, 6, 0, 3, 1, 0], [5, 0, 6, 1, 6, 0]],
                scores: [3, 2],
                turn: true,
            },
            Board {
                pebbles: [[0, 6, 0, 3, 1, 0], [0, 1, 7, 2, 7, 0]],
                scores: [3, 9],
                turn: false,
            },
            Board {
                pebbles: [[0, 6, 0, 0, 2, 1], [0, 1, 7, 2, 7, 0]],
                scores: [4, 9],
                turn: false,
            },
        ];
        let moves: Vec<i16> = vec! [
            5,
            1,
            5,
            2,
            0,
            3,
        ];

        let mut board = boards[0].clone();

        for i in 0..boards.len()-1 {
            make_move(&mut board, moves[i]);
            
            if board != boards[i+1] {
                println!("expected:");
                print_board(&boards[i+1]);
                println!("actual:, move: {}", moves[i]);
                print_board(&board);
                println!("previous");
                print_board(&boards[i]);

                panic!();
            }
        }
    }

    #[test]
    fn gameover() {
        let boards = vec! [
            Board { 
                pebbles: [[4, 4, 0, 1, 0, 10], [4, 4, 4, 0, 4, 4]],
                scores: [0, 0],
                turn: false,
            }, 
            Board {
                pebbles: [[0, 0, 0, 0, 0, 0], [5, 5, 5, 0, 5, 5]],
                scores: [3, 0],
                turn: true,
            },
            Board {
                pebbles: [[5, 5, 0, 1, 0, 0], [0, 0, 0, 0, 0, 0]],
                scores: [3, 1],
                turn: true,
            },
            Board {
                pebbles: [[6, 6, 1, 2, 1, 0], [5, 0, 6, 1, 6, 0]],
                scores: [3, 2],
                turn: false,
            },
        ];
        let expected_output = vec! [ false, true, true, false ];

        for i in 0..boards.len() {
            let board = &boards[i];
            assert_eq!(is_gameover(board), expected_output[i]);
        }
    }
}

fn main() {
    use std::time::Instant;

    let mut board = Board {
        pebbles: [[4, 4, 4, 4, 4, 4], [4, 4, 4, 4, 4, 4]],
        scores: [0, 0],
        turn: false,
    };

    for depth in 1..15 {
        let timer = Instant::now();
        let num_nodes = perft(&mut board, depth);

        println!("perft depth {}: {} nodes, {:.2?}", depth, num_nodes, timer.elapsed());
    }
}
