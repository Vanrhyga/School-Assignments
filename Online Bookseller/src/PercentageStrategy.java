public class PercentageStrategy implements IPricingStrategy{
    private double discountPercentage;
    private StrategyType strategyType=StrategyType.PERCENTAGE;
    private int strategyNumber;
    @Override
    public double getSubTotal(SaleLineItem saleLineItem) {
        return saleLineItem.getProdSpec().getPrice()*(1-discountPercentage);
    }
    @Override
    public StrategyType getStrategyType() {
        return strategyType;
    }
    @Override
    public int getStrategyNumber() {
        return strategyNumber;
    }
    @Override
    public String toString() {
        return String.format("%.0f",100*(1-discountPercentage))+"%折优惠";
    }
    @Override
    public void setStrategyNumber(int strategyNumber) {
        this.strategyNumber = strategyNumber;
    }
    public void setDiscountPercentage(double discountPercentage) {
        this.discountPercentage = discountPercentage;
    }
    public double getDiscountPercentage() {
        return discountPercentage;
    }
    public PercentageStrategy(int strategyNumber, double discountPercentage){
        this.strategyNumber=strategyNumber;
        this.discountPercentage=discountPercentage;
    }
}
