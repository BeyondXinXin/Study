fn main() {
    println!("Hello, world!");

    let mut s = String::from("hello");
    s.push_str(",word!");
    println!("{}", s);

    let s1 = String::from("hello");
    // let s2 = s1;
    let s2 = s1.clone();
    println!("{}-{}", s1, s2);
}
