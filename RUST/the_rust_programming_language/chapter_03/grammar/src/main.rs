// Rust权威指南第三章，通用编程概念

const MAX_POINTS: u32 = 100_000;

fn main() {
    println!("----------------- {}", MAX_POINTS);
    // 变量与可变性
    let mut x = 5;
    println!("The value of x is {}", x);
    x = 6;
    println!("The value of x is {}", x);
    let y = 5;
    let y = y + 1;
    let y = y * 2;
    println!("The value of y is {}", y);
    let spaces = "       ";
    println!("spaces:{},spaces len:{}", spaces, spaces.len());
    let spaces = spaces.len();
    println!("spaces:{}", spaces);

    // 数据类型(复合类型)
    let tup: (i32, f64, u8, char) = (1, 1.0, 1, 'a');
    println!("a:{},b:{},c:{},d:{}", tup.0, tup.1, tup.2, tup.3);
    let (a, b, c, d) = tup;
    println!("a:{},b:{},c:{},d:{}", a, b, c, d);

    let array1 = [1, 2, 3, 4];
    let array2 = [6; 4];
    println!("{}、{}、{}、{}", array1[0], array1[1], array1[2], array1[3]);
    println!("{}、{}、{}、{}", array2[0], array2[1], array2[2], array2[3]);

    another_function();
    another_function_1(10);
    another_function_2(10, 10);

    // 语句和表达式
    let y = {
        let x = 3;
        x + 10 + 20
    };
    println!("{}", y);

    println!("{}", five());

    // 控制流 if
    let a = 10;
    let b = true;

    if a < 5 {
        println!("a<5");
    } else if a < 8 {
        println!("a<8");
    } else {
        println!("a>=82");
    }

    let mut a = if b { 15 } else { 16 };
    a = a + 1;
    println!("{}", a);

    // 控制流 循环 loop while for
    let mut i = 0;
    loop {
        println!("i:{}", i);
        i = i + 1;
        if i > 10 {
            break;
        }
    }

    while i >= 0 {
        println!("i:{}", i);
        i = i - 1;
    }

    let array = [10; 4];
    for i in array {
        println!("i is :{}", i);
    }

    for i in (1..5).rev() {
        println!("i is :{}", i);
    }
}

// 函数
fn another_function() {
    println!("Another function.");
}

fn another_function_1(x: i32) {
    println!("x is {}", x);
}

fn another_function_2(x: i32, y: i32) {
    println!("x、y is {}、{}", x, y);
}

fn five() -> i32 {
    5
}
