use std::io::prelude::*;


pub fn write_header(mut file:std::fs::File)-> std::io::Result<()> {  
    file.write(b"/****************************************************************************\n")?;
    file.write(b"** Resource object code\n")?;
    file.write(b"\n")?;
    file.write(b"** Created by: The Resource Compiler for Qt version 5.9.9")?;
    file.write(b"\n**\n")?;
    file.write(b"** WARNING! All changes made in this file will be lost!\n")?;
    file.write(b"*****************************************************************************/\n\n")?;
    Ok(())
}








