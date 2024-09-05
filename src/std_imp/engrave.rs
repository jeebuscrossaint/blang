use crate::backend::open;
use std::fs;
use regex::Regex;

/*
pub fn engrave() {
    if let Some(file_path) = open::get_file_path() {
        match fs::read_to_string(&file_path) {
            Ok(contents) => {
                let re = Regex::new(r#"engrave\("([^"]*)"\)"#).unwrap();
                let mut engraved_strings: Vec<String> = Vec::new();

                for cap in re.captures_iter(&contents) {
                    if let Some(matched) = cap.get(1) {
                        engraved_strings.push(matched.as_str().to_string());
                    }
                }

                for string in engraved_strings {
                    println!("{}", string);
                }
            }
            Err(e) => println!("Error reading file: {}", e),
        }
    } else {
        println!("No file path set.");
    }
}*/
// src/std_imp/engrave.rs
pub fn engrave(text: &str) {
    let processed_text = text.replace("\\n", "\n").replace("\\\"", "\"");
    println!("{}", processed_text);
}