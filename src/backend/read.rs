// src/backend/read.rs
use crate::backend::open;
use std::fs;

pub fn read_file_path() {
    if let Some(file_path) = open::get_file_path() {
        match fs::read_to_string(&file_path) {
            Ok(contents) => println!("{}", contents),
            Err(e) => println!("Error reading file: {}", e),
        }
    } else {
        println!("No file path set.");
    }
}