import java.util.ArrayList;

public class BookCatalog {
    private ArrayList<BookSpecification> books;
    public void addBook(BookSpecification book){
        books.add(book);
    }
    public ArrayList<BookSpecification> getBooks(){
        return books;
    }
    public BookSpecification getBookSpecification(String isdn){
        for(BookSpecification book:books)
            if(book.getIsbn().equals(isdn))
                return book;
        return null;
    }
    public BookCatalog(){
        books=new ArrayList<>();
    }
}
