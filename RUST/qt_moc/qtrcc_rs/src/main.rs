use std::fs::File;
use std::{path::Path};
use structopt::StructOpt;
use walkdir::WalkDir;

mod resource_library;

#[derive(Debug, StructOpt)]
struct App {
    #[structopt(default_value = "data/resources.qrc")]
    inputs: String,

    /// Write output to <file> rather than stdout. 将输出写入<file>而不是标准输出。
    #[structopt(short = "o", long = "output", default_value = "./data/test.cpp")]
    output: String,

    /// Output a resource file containing all files from the current directory. 输出包含当前目录中所有文件的资源文件。
    #[structopt(long, default_value = "")]
    project: String,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let App {
        inputs,
        output,
        project,
    } = App::from_args();

    println!("{}-{}-{}", inputs, output, project);

    if !project.is_empty() {
        create_project(&project);
    };

    let file = File::create(output)?;
    resource_library::write_header(file)?;

    Ok(())
}

fn dump_recursive(dir: &str) {
    for e in WalkDir::new(dir).into_iter().filter_map(|e| e.ok()) {
        if e.metadata().unwrap().is_file() {
            println!("<file>{}</file>", e.path().display());
        }
    }
}

fn create_project(dir: &str) {
    if !Path::new(dir).exists() {
        println!("Project file does not exist '{}'", dir);
        return;
    }

    println!("<!DOCTYPE RCC><RCC version=\"1.0\">");
    println!("<qresource>");
    dump_recursive(dir);
    println!("</qresource>");
    println!("</RCC>");
}
