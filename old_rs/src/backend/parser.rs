// src/backend/parser.rs
use regex::Regex;

#[derive(Debug)]
pub enum Token {
    Engrave(String),
    Other(String),
}

pub fn tokenize(contents: &str) -> Vec<Token> {
    let re = Regex::new(r#"engrave\("((?:[^"\\]|\\.)*)"\)"#).unwrap();
    let mut tokens = Vec::new();

    let mut last_end = 0;
    for cap in re.captures_iter(contents) {
        if let Some(matched) = cap.get(1) {
            // Add any text before the match as an "Other" token
            if cap.get(0).unwrap().start() > last_end {
                tokens.push(Token::Other(contents[last_end..cap.get(0).unwrap().start()].to_string()));
            }
            // Add the "Engrave" token
            tokens.push(Token::Engrave(matched.as_str().to_string()));
            last_end = cap.get(0).unwrap().end();
        }
    }
    // Add any remaining text as an "Other" token
    if last_end < contents.len() {
        tokens.push(Token::Other(contents[last_end..].to_string()));
    }

    tokens
}

// src/backend/parser.rs
use crate::std_imp::engrave::engrave;

pub fn parse_and_execute(tokens: Vec<Token>) {
    for token in tokens {
        match token {
            Token::Engrave(text) => engrave(&text),
            Token::Other(_) => {} // Ignore other tokens for now
        }
    }
}