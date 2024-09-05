mod backend;
mod std_imp;
use backend::open::given;
use backend::read::read_file_path;


fn main() {
    given();
    read_file_path();
}