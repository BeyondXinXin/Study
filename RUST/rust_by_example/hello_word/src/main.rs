use std::fmt::{self, Display};

fn main() {
    formatted_print();
    formatted_debug_print();
    formatted_display_print();
    formatted_list_print();
    formatting();
}

fn formatted_print() {
    println!("Hello, world!");
    println!("I'm a Rustanean!");

    println!("{} days", 31);

    println!("{0},this is {1}.this is {0}", "Alice", "Bob");

    println!(
        "{subject}--{verb}--{object}",
        object = "the lazy dog",
        subject = "the quick brown fox",
        verb = "jumps over"
    );

    println!("{} of {:b} people kenw binary,the other half don't", 1, 10);

    println!("--{number:>width$}--", number = 1, width = 6);

    println!("{number:<0width$}", number = 1, width = 6);

    println!("My name is {0},{1} {0}", "Bond", "James");

    #[derive(Debug)]
    struct Structure(i32);

    println!("This struct '{:?}' won't print ...", Structure(3));

    let pi = 3.1415926;
    println!("Pi is roughly by {:.*}", 3, pi);
}

fn formatted_debug_print() {
    struct UnPrintable(i32);

    #[derive(Debug)]
    struct Printable(i32);

    #[derive(Debug)]
    struct Structure(i32);

    #[derive(Debug)]
    struct Deep(Structure);

    println!("{:?} months in a year.", 12);
    println!(
        "{1:?} {0:?} is the {actor:?} name.",
        "Slater",
        "Christion",
        actor = "actor's"
    );

    println!("Now {:?} will print!", Structure(3));
    println!("Now {:?} will print!", Deep(Structure(7)));

    #[derive(Debug)]
    struct Person<'a> {
        name: &'a str,
        age: u8,
    }

    let name = "Yaoxin";
    let age = 26;
    let peter = Person { name, age };

    println!("{:?}--{:#?}", peter, peter);
}

fn formatted_display_print() {
    struct Structure(i32);

    impl fmt::Display for Structure {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            write!(f, "{}", self.0)
        }
    }

    #[derive(Debug)]
    struct MinMax(i64, i64);
    impl fmt::Display for MinMax {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            write!(f, "{} , {}", self.0, self.1)
        }
    }

    #[derive(Debug)]
    struct Point2D {
        x: f64,
        y: f64,
    }

    impl fmt::Display for Point2D {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            write!(f, "x: {} , y: {}", self.x, self.y)
        }
    }

    let minmax = MinMax(0, 14);
    println!("Compare structures:");
    println!("Display: {}", minmax);
    println!("Debug: {:?}", minmax);

    let big_range = MinMax(-300, 300);
    let small_range = MinMax(-3, 3);

    println!(
        "The big range is {big} and the small is {small}",
        big = big_range,
        small = small_range
    );

    let point = Point2D { x: 3.3, y: 7.2 };

    println!("Compare structures:");
    println!("Display: {}", point);
    println!("Debug: {:?}", point);

    #[derive(Debug)]
    struct Complex {
        real: f64,
        imag: f64,
    }

    impl fmt::Display for Complex {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            write!(f, "{}  + {}i", self.real, self.imag)
        }
    }

    let complex = Complex {
        real: 3.3,
        imag: 7.2,
    };
    println!("Compare structures:");
    println!("Display: {}", complex);
    println!("Debug: {:?}", complex);
}

fn formatted_list_print() {
    struct List(Vec<i32>);

    impl fmt::Display for List {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            let vec = &self.0;
            write!(f, "[")?;
            for (count, v) in vec.iter().enumerate() {
                if count != 0 {
                    write!(f, ", ")?;
                }
                write!(f, "{1} : {0}", v, count)?;
            }
            write!(f, "]")
        }
    }

    let vec = List(vec![4, 3, 2, 1]);
    println!("{}", vec);
}

fn formatting() {
    struct City {
        name: &'static str,
        lat: f32,
        lon: f32,
    }

    impl Display for City {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            let lat_c = if self.lat > 0.0 { 'N' } else { 'S' };
            let lon_c = if self.lon > 0.0 { 'E' } else { 'W' };

            write!(
                f,
                "{}: {:.3}°{} {:.3}°{}",
                self.name,
                self.lat.abs(),
                lat_c,
                self.lon.abs(),
                lon_c
            )
        }
    }

    #[derive(Debug)]
    struct Color {
        red: u8,
        green: u8,
        blue: u8,
    }

    impl Display for Color {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            write!(
                f,
                "RGB ({},{},{}) 0x{:02X?}{:02X?}{:02X?}",
                self.red, self.green, self.blue, self.red, self.green, self.blue
            )
        }
    }

    let citys = vec![
        City {
            name: "Doblin",
            lat: 53.347778,
            lon: -123.1,
        },
        City {
            name: "Oslo",
            lat: 53.347778,
            lon: -123.1,
        },
        City {
            name: "Vancouver",
            lat: 53.347778,
            lon: -123.1,
        },
    ];

    for city in citys {
        println!("{}", city);
    }

    let colors = vec![
        Color {
            red: 128,
            green: 255,
            blue: 90,
        },
        Color {
            red: 0,
            green: 3,
            blue: 254,
        },
        Color {
            red: 0,
            green: 0,
            blue: 0,
        },
    ];

    for color in colors {
        println!("{:?}", color);
        println!("{}", color);
    }
}
