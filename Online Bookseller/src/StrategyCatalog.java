import java.util.HashMap;

public class StrategyCatalog {
    private HashMap<BookType,IPricingStrategy> strategies;
    HashMap<BookType,IPricingStrategy> getStrategies(){
        return strategies;
    }
    public BookType returnBookType(int strategyNumer){
        for(HashMap.Entry<BookType,IPricingStrategy> entry:strategies.entrySet())
            if(entry.getValue().getStrategyNumber()==strategyNumer)
                return entry.getKey();
        return null;
    }
    public StrategyType returnStrategyType(int strategyNumber){
        BookType bookType=returnBookType(strategyNumber);
        return strategies.get(bookType).getStrategyType();
    }
    boolean hasStrategy(int strategyNumber){
        if(returnBookType(strategyNumber)==null)
            return false;
        return true;
    }
    public IPricingStrategy returnStrategies(int strategyNumber){
        IPricingStrategy simpleStrategy=strategies.get(returnBookType(strategyNumber));
        return simpleStrategy;
    }
    public StrategyCatalog(){
        strategies=new HashMap<>();
    }
}
