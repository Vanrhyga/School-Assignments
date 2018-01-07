public class CompositeBestForCustomer extends CompositeStrategy{
    @Override
    public double getSubTotal(SaleLineItem saleLineItem) {
        double min=Double.MAX_VALUE;
        double price;
        for(IPricingStrategy strategy:strategies){
            price=strategy.getSubTotal(saleLineItem);
            min=Double.min(price,min);
        }
        return min;
    }
    public CompositeBestForCustomer(int strategyNumber){
        super(strategyNumber);
    }
}
