const NUM_HOLES: i32 = 6;

struct Board {
    pebbles: [[i32; NUM_HOLES as usize]; 2],
    scores: [i32; 2],
    turn: bool,
}

fn print_board(board: &Board) {
    for i in 0..NUM_HOLES {
        println!("{} {}", board.pebbles[0][i as usize], board.pebbles[1][(NUM_HOLES-i-1) as usize]);
    }
    println!("scores: {}, {}", board.scores[0], board.scores[1]);
    println!("turn: {}", board.turn);
}

fn make_move(board: &mut Board, mmove: u16) {
    let mut idx: i32 = (mmove & 0x7).into();
    board.pebbles[board.turn as usize][idx as usize] = 0;

    let mut num_pebbles = mmove >> 3;
    let mut side_placing_on = board.turn;

    while num_pebbles > 0 {
        idx += 1;
        num_pebbles -= 1;

        if idx == NUM_HOLES {
            if side_placing_on == board.turn {
                board.scores[board.turn as usize] += 1;
            }

            idx = -1; // idx will be incremented to 0 before being used.
            side_placing_on = !side_placing_on;
        }
        else {
            board.pebbles[side_placing_on as usize][idx as usize] += 1;
        }
    }
    board.turn = !board.turn;
}

fn main() {
    let mut board = Board {
        pebbles: [[4, 4, 4, 4, 4, 4], [4, 4, 4, 4, 4, 4]],
        scores: [0, 0],
        turn: false,
    };
    print_board(&board);
    let new_move: u16 = 0b100101;
    make_move(&mut board, new_move);
    print_board(&board);
}
