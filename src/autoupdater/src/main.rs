#![windows_subsystem = "windows"]

slint::include_modules!();

//use i_slint_backend_winit::WinitWindowAccessor;
use std::env;
use reqwest;
use std::fs;
use std::path::Path;
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Debug)]
struct VersionAsset {
    name: String,
    browser_download_url: String
}

#[derive(Serialize, Deserialize, Debug)]
struct VersionResponse {
    tag_name: String,
    assets: Vec<VersionAsset>
}

async fn get_latest_version() -> VersionResponse {
    let client = reqwest::Client::builder()
        .user_agent("Anilibria.Qt AutoUpdater")
        .build()
        .expect("Builder not created!");
    let response = client.get("https://api.github.com/repos/anilibria/anilibria-winmaclinux/releases/latest")
        .send()
        .await
        .unwrap();
    match response.status() {
        reqwest::StatusCode::OK => {
            match response.json::<VersionResponse>().await {
                Ok(parsed) => {
                    return parsed;
                },
                Err(_) => panic!("Something went wrong while parsing github data"),
            };
        }
        other => {
            panic!("Something went wrong with request: {:?}", other);
        }
    };
}

/*
// you need this in your cargo.toml
// reqwest = { version = "0.11.3", features = ["stream"] }
// futures-util = "0.3.14"
// indicatif = "0.15.0"
use std::cmp::min;
use std::fs::File;
use std::io::Write;

use reqwest::Client;
use indicatif::{ProgressBar, ProgressStyle};
use futures_util::StreamExt;

pub async fn download_file(client: &Client, url: &str, path: &str) -> Result<(), String> {
    // Reqwest setup
    let res = client
        .get(url)
        .send()
        .await
        .or(Err(format!("Failed to GET from '{}'", &url)))?;
    let total_size = res
        .content_length()
        .ok_or(format!("Failed to get content length from '{}'", &url))?;
    
    // Indicatif setup
    let pb = ProgressBar::new(total_size);
    pb.set_style(ProgressStyle::default_bar()
        .template("{msg}\n{spinner:.green} [{elapsed_precise}] [{wide_bar:.cyan/blue}] {bytes}/{total_bytes} ({bytes_per_sec}, {eta})")
        .progress_chars("#>-"));
    pb.set_message(&format!("Downloading {}", url));

    // download chunks
    let mut file = File::create(path).or(Err(format!("Failed to create file '{}'", path)))?;
    let mut downloaded: u64 = 0;
    let mut stream = res.bytes_stream();

    while let Some(item) = stream.next().await {
        let chunk = item.or(Err(format!("Error while downloading file")))?;
        file.write_all(&chunk)
            .or(Err(format!("Error while writing to file")))?;
        let new = min(downloaded + (chunk.len() as u64), total_size);
        downloaded = new;
        pb.set_position(new);
    }

    pb.finish_with_message(&format!("Downloaded {} to {}", url, path));
    return Ok(());
*/

#[tokio::main]
async fn main() {
    let path = Path::new("savedversion");
    let mut saved_version = "".to_string();
    if path.exists() {
        saved_version = fs::read_to_string(path).expect("Should have been able to read the file");
    }
    let version = get_latest_version().await;
    if version.tag_name != saved_version {
        println!("New version {} is available!", version.tag_name);
        
        //TODO: need download latest version
        match env::consts::OS {
            "windows" => println!("windows"),
            "macos" => println!("macos"),
            "linux" => println!("linux"),
            _ => println!("anything"),
        }

        fs::write("savedversion", version.tag_name).expect("Unable to write version file");
    }
    
}

/*#[tokio::main]
async fn main() {
    /* let app:App = App::new().unwrap();

    app.window().with_winit_window(|winit_win: &winit::window::Window| {
        //winit_win.set_fullscreen(Some(winit::window::Fullscreen::Borderless(winit_win.current_monitor())));
        let monitor = winit_win.current_monitor().unwrap();
        let width = monitor.size().width / 2;
        let height = monitor.size().height / 2;
        let window_width = 500 / 2;
        let window_height = 430 / 2;
        let center_width: i32 = (width - window_width) as i32;
        let center_height: i32 = (height - window_height) as i32;
        app.window().set_position(slint::PhysicalPosition::new(center_width, center_height));
    });

    app.run().unwrap(); */

}*/
