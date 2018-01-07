public class PricingStrategyFactory {
    private PricingStrategyFactory instance;
    private StrategyCatalog catalog;
    public PricingStrategyFactory getInstance() {
        if(instance==null)
            instance=new PricingStrategyFactory();
        return instance;
    }
    public IPricingStrategy getPricingStrategy(BookType type){
        IPricingStrategy strategy=catalog.getStrategies().get(type);
        if(strategy==null)
            return new NoDiscountStrategy();
        else
            return strategy;
    }
    public void setCatalog(StrategyCatalog catalog){
        this.catalog=catalog;
    }
}
