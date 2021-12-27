fn main() {
    // 自己先写了一份
    // 温度转换
    temperature_conversion();
    // fib序列
    fibonacci_sequence(10);
    // 歌词
    print_lyric();

    // 看了下别人的
    loop_fip();
    print_lyric2();
}

fn temperature_conversion() {
    println!(
        "fahrenheit: {} ==> centigrade: {}",
        70f64,
        fahrenheit_to_centigrade(70f64)
    );

    println!(
        "centigrade: {} ==> fahrenheit: {}",
        20f64,
        centigrade_to_fahrenheit(20f64)
    );
}

fn fahrenheit_to_centigrade(fahrenheit: f64) -> f64 {
    (fahrenheit - 32f64) * 5f64 / 9f64
}

fn centigrade_to_fahrenheit(centigrade: f64) -> f64 {
    centigrade * 9f64 / 5f64 + 32f64
}

fn fibonacci_sequence(size: i32) {
    println!("size:{}", size);
    let mut n_back = 0;
    let mut n = 1;
    if size == 0 {
        println!("0/{}:{}", size, n_back);
    } else if size == 1 {
        println!("0/{}:{}", size, n_back);
        println!("1/{}:{}", size, n);
    } else {
        println!("0/{}:{}", size, n_back);
        println!("1/{}:{}", size, n);
        let mut i = 2;
        while i < size {
            let add = n_back + n;
            n_back = n;
            n = add;
            println!("{}/{}:{}", i, size, n);
            i = i + 1;
        }
    }
}

fn print_lyric() {
    let days = [
        "一", "二", "三", "四", "五", "六", "七", "八", "九", "十", "十一", "十二",
    ];
    let gifts = [
        "一只站在梨树上的鹧鸪鸟。",
        "两只鸠、",
        "三只法国母鸡、",
        "四只鸣唱的鸟儿、",
        "五只金戒指、",
        "六只生蛋的鹅、",
        "七只游水的天鹅、",
        "八位挤奶的佣妇、",
        "九位跳舞的女士、",
        "十个跳跃的男人、",
        "十一个吹风笛的风笛手、",
        "十二个打鼓的鼓手、",
    ];

    for i in 1..13 {
        let mut data: String = "".to_string();
        for j in (0..i).rev() {
            if i != 1 && j == 0 {
                data += "及";
                data += gifts[j];
            } else {
                data += gifts[j];
            }
        }
        println!("在圣诞节的第{}天，我的真爱送我：{}", days[i - 1], data);
    }
}

// ---------------------------------------------------
// https://blog.csdn.net/weixin_44808809/article/details/103651836
fn fib(n: u32) -> u32 {
    match n {
        0 => 1,
        1 => 1,
        _ => fib(n - 1) + fib(n - 2),
    }
}

fn loop_fip() {
    let mut n: u32 = 0;
    loop {
        n += 1;
        if n > 30 {
            break;
        } else {
            println!("{}: {}", n, fib(n));
        }
    }
}

// ---------------------------------------------------
// https://www.cnblogs.com/qisong/p/14516414.html

fn print_lyric2() {
    let count = [
        "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth",
        "tenth", "eveth", "Twelve",
    ];

    println!("The Twelve Days of christmas - Songtime Kids");

    for i in 0..count.len() {
        println!(
            "On the {} day of Christmas 
        \rMy true love sent to me",
            count[i]
        );
        if i == 0 {
            println!("A partridge in a pear tree");
            continue;
        }
        for j in 0..=i {
            match i - j {
                0 => println!("And a partridge in a pear tree"),
                1 => println!("Two turtle doves"),
                2 => println!("Three French hens"),
                3 => println!("Four calling birds"),
                4 => println!("Five golden Rings"),
                5 => println!("Six geese a-laying"),
                6 => println!("Seven swans a-swimming"),
                7 => println!("Eight maids a-milking"),
                8 => println!("Nine ladies dancing"),
                9 => println!("Ten Lords a Leaping"),
                10 => println!("Eleven Pipers Piping"),
                11 => println!("Twelve Drummers Drumming"),
                _ => (),
            }
        }
    }
}
