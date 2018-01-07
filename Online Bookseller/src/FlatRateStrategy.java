public class FlatRateStrategy implements IPricingStrategy{
    private int discountPerBook;
    private StrategyType strategyType=StrategyType.FLATRATE;
    private int strategyNumber;
    @Override
    public double getSubTotal(SaleLineItem saleLineItem) {
        double price=saleLineItem.getProdSpec().getPrice()-discountPerBook;
        if(price>0)
            return price;
        return 0;
    }
    @Override
    public StrategyType getStrategyType() {
        return strategyType;
    }
    @Override
    public String toString() {
        return "每本减"+String.valueOf(discountPerBook)+"元";
    }
    public void setStrategyNumber(int strategyNumber) {
        this.strategyNumber = strategyNumber;
    }
    public void setDiscountPerBook(int discountPerBook){
        this.discountPerBook=discountPerBook;
    }
    public int getStrategyNumber(){
        return strategyNumber;
    }
    public FlatRateStrategy(int strategyNumber,int discountPerBook){
        this.strategyNumber=strategyNumber;
        this.discountPerBook=discountPerBook;
    }
}
