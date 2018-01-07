public interface IPricingStrategy {
    double getSubTotal(SaleLineItem saleLineItem);
    StrategyType getStrategyType();
    int getStrategyNumber();
    String toString();
}
