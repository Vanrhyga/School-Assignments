public class BookSpecification {
    private String isbn;
    private String title;
    private double price;
    private BookType type;
    public String getIsbn(){
        return isbn;
    }
    public String getTitle(){
        return title;
    }
    public double getPrice(){
        return price;
    }
    public BookType getType(){
        return type;
    }
    public BookSpecification(String isbn,String title,double price,BookType type){
        this.isbn=isbn;
        this.title=title;
        this.price=price;
        this.type=type;
    }
}
