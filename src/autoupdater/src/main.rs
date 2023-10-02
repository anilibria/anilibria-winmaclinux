#![windows_subsystem = "windows"]

slint::include_modules!();

use i_slint_backend_winit::WinitWindowAccessor;

fn main() {
    let app:App = App::new().unwrap();

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

    app.run().unwrap();
}
