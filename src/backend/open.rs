use std::sync::Mutex;

static mut FILE_PATH: Option<Mutex<String>> = None;

pub fn look() {
    let mut found = false;
    if let Ok(entries) = std::fs::read_dir(".") {
        for entry in entries {
            if let Ok(entry) = entry {
                if let Some(file_name) = entry.file_name().to_str() {
                    if file_name == "main.kid" {
                        // Found the file, do something with it
                        println!("\x1b[32mFound main.kid in the current directory!\x1b[0m");
                        unsafe {
                            FILE_PATH = Some(Mutex::new("main.kid".to_string()));
                        }
                        found = true;
                        break;
                    }
                }
            }
        }
    }
    if !found {
        println!("\x1b[31mYo, make a file named 'main.kid' or provide a path to a file explicitly.\x1b[0m");
    }
}

pub fn given() {
    if let Some(file_path) = std::env::args().nth(1) {
        println!("\x1b[32mRunning file: {}\x1b[0m", file_path);
        unsafe {
            FILE_PATH = Some(Mutex::new(file_path));
        }

    } else {
        look();
    }
}

pub fn get_file_path() -> Option<String> {
    unsafe {
        FILE_PATH.as_ref().map(|mutex| mutex.lock().unwrap().clone())
    }
}