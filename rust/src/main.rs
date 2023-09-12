use std::io::{stdout, Write};

use chrono::prelude::*;
use clap::Parser;
use crossterm::{cursor, execute, queue, terminal};
use termion::terminal_size;

#[derive(Parser, Debug)]
struct TrackerArgs {
    #[arg(short, long)]
    start_date: i64,

    #[arg(short, long)]
    end_date: i64,

    #[arg(short = 'f', long)]
    start_location: String,

    #[arg(short = 't', long)]
    end_location: String,
}

const PLANE_TAKEOFF: &str = "";
const PLANE_LANDING: &str = "";
const PLANE_FLYING: &str = "";

fn main() -> std::io::Result<()> {
    let args = TrackerArgs::parse();

    let _ = ctrlc::set_handler(|| {
        let mut stdout = stdout();
        queue!(stdout, cursor::RestorePosition).expect("");
        queue!(stdout, terminal::Clear(terminal::ClearType::CurrentLine)).expect("");
        execute!(stdout, cursor::Show).expect("");

        let _ = stdout.flush();
        std::process::exit(0);
    });

    let start_date = DateTime::parse_from_str(format!("{}", args.start_date).as_str(), "%s")
        .expect("needs a valid timestamp");
    let end_date = DateTime::parse_from_str(format!("{}", args.end_date).as_str(), "%s")
        .expect("needs a valid timestamp");

    let now = Local::now();
    let duration = (end_date - start_date).num_seconds();
    let elapsed = (now - start_date.with_timezone(&Local)).num_seconds();
    if elapsed > duration {
        match (now - end_date.with_timezone(&Local)).num_hours() {
            dt if dt > 1 => println!("Flight already landed {} hours ago", dt),
            _ => println!("Flight just landed"),
        }

        return Ok(());
    } else if elapsed < 0 {
        match (start_date.with_timezone(&Local) - now).num_hours() {
            dt if dt > 1 => println!("Flight has not departed, check back in {} hours", dt),
            _ => println!("Flight is preparing to depart, check back in a few minutes"),
        }

        return Ok(());
    }

    let begin = format!(
        "({}) {} ",
        start_date.with_timezone(&Local).format("%H:%M").to_string(),
        args.start_location
    );
    let end = format!(
        " {} ({})",
        args.end_location,
        end_date.with_timezone(&Local).format("%H:%M").to_string()
    );

    loop {
        let (width, _height) = terminal_size().unwrap();
        let width = width as usize;
        let now = Local::now();
        let elapsed = (now - start_date.with_timezone(&Local)).num_seconds();

        if elapsed > duration {
            println!("Flight complete");
            break;
        }

        let mut line = ".".repeat(width);
        line.replace_range(..begin.len(), &begin);
        line.replace_range(width - end.len().., &end);
        let plane_range = (1 + begin.len(), width - end.len() - 1);

        match elapsed as f64 / duration as f64 {
            x if x <= 0.15 => {
                let p_string = format!("{} ({:.2?}%)", PLANE_TAKEOFF, x * 100.);
                let spot = ((plane_range.1 - plane_range.0) as f64 * x / 100.).floor() as usize;
                line.replace_range(
                    1 + plane_range.0 + spot..&p_string.len() + spot + plane_range.0,
                    &p_string,
                )
            }
            x if x >= 0.85 => {
                let p_string = format!("{} ({:.2?}%)", PLANE_LANDING, x * 100.);
                let spot = ((plane_range.1 - plane_range.0) as f64 * x).floor() as usize;
                line.replace_range(
                    1 + plane_range.0 + spot..p_string.len() + spot + plane_range.0,
                    &p_string,
                )
            }
            x => {
                let p_string = format!("{} ({:.2?}%)", PLANE_FLYING, x * 100.);
                let spot = ((plane_range.1 - plane_range.0) as f64 * x).floor() as usize;
                line.replace_range(
                    begin.len() + 1 + spot..p_string.len() + begin.len() + spot,
                    &p_string,
                )
            }
        };

        let mut stdout = stdout();
        execute!(stdout, cursor::Hide)?;
        queue!(stdout, cursor::SavePosition)?;
        stdout.write_all(format!("{}", line).as_bytes())?;
        queue!(stdout, cursor::RestorePosition)?;
        stdout.flush()?;

        std::thread::sleep(std::time::Duration::from_millis(1_000));
        queue!(stdout, cursor::RestorePosition)?;
        queue!(stdout, terminal::Clear(terminal::ClearType::CurrentLine))?;
        execute!(stdout, cursor::Show)?;
    }

    Ok(())
}
