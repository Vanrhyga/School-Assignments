import java.util.ArrayList;

public abstract class CompositeStrategy implements IPricingStrategy{
    protected ArrayList<IPricingStrategy> strategies;
    private StrategyType strategyType=StrategyType.COMPOSITE;
    private int strategyNumber;
    public abstract double getSubTotal(SaleLineItem saleLineItem);
    @Override
    public StrategyType getStrategyType() {
        return strategyType;
    }
    @Override
    public String toString() {
        String s = "";
        int i=strategies.size();
        for (IPricingStrategy strategy : strategies) {
            s += String.valueOf(strategy.getStrategyNumber());
            i--;
            if(i>0)
                s+="+";
        }
        return "策略"+s;
    }
    public int getStrategyNumber() {
        return strategyNumber;
    }
    public ArrayList<IPricingStrategy> getStrategies() {
        return strategies;
    }
    public void add(IPricingStrategy strategy){
        strategies.add(strategy);
    }
    public CompositeStrategy(int strategyNumber){
        this.strategyNumber=strategyNumber;
        strategies=new ArrayList<>();
    }
}
