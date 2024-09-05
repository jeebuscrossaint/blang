// src/main.rs
mod backend;
mod std_imp;
//mod std;

use backend::open::given;
use backend::read::read_file_path;

fn main() {
    given();
    read_file_path();
}