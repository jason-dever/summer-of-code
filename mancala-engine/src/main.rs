const NUM_POCKETS: usize = 6;

#[derive(PartialEq, Debug)]
enum GameResult {
    Draw,
    P1Win,
    P2Win,
}

#[derive(Clone, PartialEq)]
struct Board {
    pebbles: [[i16; NUM_POCKETS]; 2],
    scores: [i16; 2],
    turn: bool,
}

fn make_move(board: &mut Board, mut idx: i16, game_type: char) {
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
        if game_type == 'a' && num_pebbles == 0 && idx != -1 && board.pebbles[side_placing_on as usize][idx as usize] > 1 {
            num_pebbles = board.pebbles[side_placing_on as usize][idx as usize];
            board.pebbles[side_placing_on as usize][idx as usize] = 0;
            // println!("update: {idx}");
        }
    }
    // Checks for captures
    if game_type == 'c' && side_placing_on == board.turn && board.pebbles[board.turn as usize][idx as usize] == 1 && board.pebbles[!(board.turn) as usize][(5-idx) as usize] > 0 {
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

fn get_game_result(board: &Board, game_type: char) -> GameResult {
    // It's assumed that the game is over when this fn is called.
    let difference = if game_type == 'c' {
        board.scores[0] + board.pebbles[0].iter().sum::<i16>() - board.scores[1] - board.pebbles[1].iter().sum::<i16>()
    }
    else { // Avalanche
        board.scores[0] - board.scores[1]
    };

    if difference > 0 {
        return GameResult::P1Win;
    }
    else if difference < 0 {
        return GameResult::P2Win;
    }
    else {
        return GameResult::Draw;
    }
}

fn eval(board: &Board) -> f64 {
    return (board.scores[0] - board.scores[1]) as f64;
}

struct MiniMaxOutput {
    best_move: usize,
    eval: f64,
}


fn minimax(board: &mut Board, mut alpha: f64, mut beta: f64, depth: i32, game_type: char) -> MiniMaxOutput {
    if depth == 0 {
        return MiniMaxOutput {
            best_move: 0,
            eval: eval(board),
        }
    }

    if is_gameover(board) {
        // This ensures we are winning by as much as possible and losing by 
        // as little as possible.
        let mut heuristic = board.scores[0] - board.scores[1];
        if game_type == 'c' {
            heuristic += board.pebbles[0].iter().sum::<i16>() - board.pebbles[1].iter().sum::<i16>();
        };
        return MiniMaxOutput {
            best_move: 0,
            eval: match get_game_result(board, game_type) {
                    GameResult::P1Win => 10000.0 + heuristic as f64,
                    GameResult::P2Win => -10000.0 + heuristic as f64,
                    GameResult::Draw => 0.0,
            }
        }
    }

    let mut best_idx = 0 ;
    let mut best_score = if board.turn == true { f64::MAX } else { f64::MIN };

    let original = board.clone();

    for i in 0..NUM_POCKETS {
        if board.pebbles[board.turn as usize][i] != 0 {
            make_move(board, i as i16, game_type);
            let move_score = minimax(board, alpha, beta, depth-1, game_type).eval;

            if (move_score > best_score) ^ original.turn {
                best_score = move_score;
                best_idx = i;
            }

            if original.turn == false { // Maximizer
                if move_score > beta {
                    break;
                }
                alpha = if alpha > move_score { alpha } else { move_score };
            }
            else { // Minimizer
                if move_score < alpha {
                    break;
                }
                beta = if beta < move_score { beta } else { move_score };
            }
            *board = original.clone();
        }
    }
    return MiniMaxOutput {
        best_move: best_idx,
        eval: best_score,
    };
}

fn perft(board: &mut Board, depth: i32) -> u64 {
    if depth == 0 || is_gameover(board) {
        return 1;
    }

    let mut num_nodes = 0;
    for i in 0..NUM_POCKETS {
        let num_pebbles = board.pebbles[board.turn as usize][i];

        if num_pebbles != 0 {
            let original = board.clone();
            make_move(board, i as i16, 'c');
            num_nodes += perft(board, depth-1);
            *board = original.clone();
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
            make_move(&mut board, moves[i], 'c');
            
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
    // use std::time::Instant;

    // engine_vs_engine(minimax2, minimax, 100);
    let mut board = Board {
        pebbles: [[0, 6, 4, 4, 2, 5], [3, 4, 2, 2, 0, 1]],
        scores: [0, 1],
        turn: false,
    };

    // let test = minimax(&mut board, 4);
    // println!("{}, {}", test.eval, test.best_move);
    let game_type = 'c';

    print_board(&board);
    while !is_gameover(&board) {

        let mut turn = board.turn;
        while turn == board.turn && !is_gameover(&board) {
            let best_move = minimax(&mut board, f64::MIN, f64::MAX, 16, game_type).best_move;
            println!("move: {best_move}, val: {}", board.pebbles[board.turn as usize][best_move]);
            make_move(&mut board, best_move as i16, game_type);
            print_board(&board);
        }
        turn = board.turn;
        while turn == board.turn && !is_gameover(&board) {
            let mut user_str = String::new();
            println!("your turn");
            std::io::stdin().read_line(&mut user_str).unwrap();
            let idx = user_str.trim().parse::<i16>().unwrap();
            make_move(&mut board, idx, game_type);
            print_board(&board);
        }

    }
    if game_type == 'c' {
        for i in 0..=1 {
            let mut sum = 0;

            for j in 0..board.pebbles[i].len() {
                let count = board.pebbles[i][j];
                sum += count;
                board.pebbles[i][j] = 0;
            }
            board.scores[i] += sum;
        }
    }
    print_board(&board);
    // for depth in 1..15 {
    //     let timer = Instant::now();
    //     let num_nodes = perft(&mut board, depth);

    //     println!("perft depth {}: {} nodes, {:.2?}", depth, num_nodes, timer.elapsed());
    // }
}
