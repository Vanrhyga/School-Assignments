public class SaleLineItem {
    private int copies;
    private BookSpecification prodSpec;
    private IPricingStrategy strategy;
    public double getSubTotal(){
        return strategy.getSubTotal(this)*copies;
    }
    public BookSpecification getProdSpec() {
        return prodSpec;
    }
    public int getCopies() {
        return copies;
    }
    public IPricingStrategy getStrategy() {
        return strategy;
    }
    public SaleLineItem(int copies, BookSpecification prodSpec, IPricingStrategy strategy){
        this.copies=copies;
        this.prodSpec=prodSpec;
        this.strategy=strategy;
    }
}
