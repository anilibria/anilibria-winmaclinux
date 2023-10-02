#![windows_subsystem = "windows"]

slint::include_modules!();

fn main() {
    let app:App = App::new().unwrap();
    app.run().unwrap();
}
