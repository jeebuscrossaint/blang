// src/backend/read.rs
use crate::backend::open;
use crate::backend::parser::{tokenize, parse_and_execute};
use std::fs;

pub fn read_file_path() {
    if let Some(file_path) = open::get_file_path() {
        match fs::read_to_string(&file_path) {
            Ok(contents) => {
                let tokens = tokenize(&contents);
                parse_and_execute(tokens);
            }
            Err(e) => println!("Error reading file: {}", e),
        }
    } else {
        println!("No file path set.");
    }
}