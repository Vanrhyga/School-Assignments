public interface IPricingStrategy {
    double getSubTotal(SaleLineItem saleLineItem);
    StrategyType getStrategyType();
    int getStrategyNumber();
    void setStrategyNumber(int number);
    String toString();
}
