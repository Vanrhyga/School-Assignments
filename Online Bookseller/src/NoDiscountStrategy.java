public class NoDiscountStrategy implements IPricingStrategy{
    private StrategyType strategyType=StrategyType.NO;
    @Override
    public double getSubTotal(SaleLineItem saleLineItem){
        return saleLineItem.getProdSpec().getPrice();
    }
    @Override
    public StrategyType getStrategyType() {
        return strategyType;
    }
    @Override
    public int getStrategyNumber() {
        return 0;
    }
    @Override
    public String toString() {
        return "无优惠";
    }
}
