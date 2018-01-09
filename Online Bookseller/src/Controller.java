import javax.swing.*;
import java.util.ArrayList;
import java.util.HashMap;

public class Controller implements SubjectForMain{
    private BookCatalog bookCatalog;
    private StrategyCatalog strategyCatalog;
    private Sale sale;
    private PricingStrategyFactory pricingStrategyFactory;
    private ArrayList<ObserverForMain> observerForMains;
    @Override
    public void removeObserver(ObserverForMain observer) {
        if(observerForMains.contains(observer))
            observerForMains.remove(observer);
    }
    @Override
    public void registerObserver(ObserverForMain observer) {
        observerForMains.add(observer);
    }
    @Override
    public void notifyObservers() {
        for (ObserverForMain observerForMain:observerForMains)
            observerForMain.update();
    }
    public void addBook(BookSpecification book){
        bookCatalog.addBook(book);
        notifyObservers();
    }
    public boolean addCompositeStrategy(BookType bookType,int strategyNumber,String content){
        ArrayList<Integer> numbers=new ArrayList<>();
        String[] strategies=content.split("\\|");
        CompositeStrategy compositeStrategy=new CompositeBestForCustomer(strategyNumber);
        IPricingStrategy simpleStrategy;
        if(strategies.length<2){
            JOptionPane.showMessageDialog(null,"复合策略格式有误！","错误",JOptionPane.ERROR_MESSAGE);
            return false;
        }
        else {
            for(String strategy:strategies){
                try {
                    numbers.add(Integer.parseInt(strategy));
                }
                catch (java.lang.NumberFormatException e) {
                    JOptionPane.showMessageDialog(null,"复合策略生成失败，策略参数有误！","错误",JOptionPane.ERROR_MESSAGE);
                    return false;
                }
            }
            if (strategyCatalog.getStrategies().containsKey(bookType)){
                JOptionPane.showMessageDialog(null,"复合策略生成失败，该类型图书已有优惠策略！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            if(strategyCatalog.hasStrategy(strategyNumber)){
                JOptionPane.showMessageDialog(null,"复合策略生成失败，策略号重复！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            for(Integer i:numbers){
                if(strategyCatalog.returnStrategyType(i)==StrategyType.COMPOSITE){
                    JOptionPane.showMessageDialog(null,"复合策略生成失败，添加了非简单策略！","错误",JOptionPane.ERROR_MESSAGE);
                    return false;
                }
                simpleStrategy=strategyCatalog.getStrategies().get(strategyCatalog.returnBookType(i));
                if(compositeStrategy.strategies.contains(simpleStrategy)){
                    JOptionPane.showMessageDialog(null,"复合策略生成失败，添加了重复策略！","错误",JOptionPane.ERROR_MESSAGE);
                    return false;
                }
                else
                    compositeStrategy.add(simpleStrategy);
            }
            strategyCatalog.getStrategies().put(bookType,compositeStrategy);
            return true;
        }
    }
    public boolean addSimpleStrategy(BookType bookType,StrategyType strategyType,int strategyNumber,String content){
        int discountPerBook;
        double discountPercentage;
        if(strategyType==StrategyType.FLATRATE){
            try {
                discountPerBook=Integer.parseInt(content);
            }
            catch (java.lang.NumberFormatException e){
                JOptionPane.showMessageDialog(null,"添加策略失败，参数格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            FlatRateStrategy flatRateStrategy=new FlatRateStrategy(strategyNumber,discountPerBook);
            if(strategyCatalog.getStrategies().containsKey(bookType)){
                JOptionPane.showMessageDialog(null,"添加策略失败，该类型图书已有优惠策略！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            else if(strategyCatalog.hasStrategy(strategyNumber)){
                JOptionPane.showMessageDialog(null,"添加策略失败，策略号重复！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            else {
                strategyCatalog.getStrategies().put(bookType,flatRateStrategy);
                return true;
            }
        }
        else if(strategyType==StrategyType.PERCENTAGE){
            try {
                discountPercentage=Double.parseDouble(content);
            }
            catch (java.lang.NumberFormatException e){
                JOptionPane.showMessageDialog(null,"添加策略失败，参数格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            discountPercentage/=100;
            PercentageStrategy percentageStrategy=new PercentageStrategy(strategyNumber,discountPercentage);
            if(strategyCatalog.getStrategies().containsKey(bookType)){
                JOptionPane.showMessageDialog(null,"添加策略失败，该类型图书已有优惠策略！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            else if(strategyCatalog.hasStrategy(strategyNumber)){
                JOptionPane.showMessageDialog(null,"添加策略失败，策略号重复！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            else {
                strategyCatalog.getStrategies().put(bookType,percentageStrategy);
                return true;
            }
        }
        else
            return false;
    }
    public ArrayList<IPricingStrategy> returnCompositeStrategies(int number){
        HashMap<BookType,IPricingStrategy> strategies=strategyCatalog.getStrategies();
        IPricingStrategy simpleStrategy = returnStrategies(number);
        ArrayList<IPricingStrategy> list = new ArrayList<>();
        for(HashMap.Entry<BookType,IPricingStrategy> entry: strategies.entrySet()){
            IPricingStrategy strategy = entry.getValue();
            if(strategy.getStrategyType() == StrategyType.COMPOSITE){
                CompositeStrategy compositeStrategy = (CompositeStrategy)strategy;
                if(compositeStrategy.strategies.contains(simpleStrategy)){
                    list.add(compositeStrategy);
                }
            }
        }
        return list;
    }
    public void deleteStrategy(int strategyNumber){
        BookType bookType=strategyCatalog.returnBookType(strategyNumber);
        strategyCatalog.getStrategies().remove(bookType);
    }
    public boolean updateStrategy(BookType bookType,StrategyType strategyType,int strategyNumber,String content){
        if(strategyType==StrategyType.FLATRATE){
            int discountPerBook;
            try {
                discountPerBook=Integer.parseInt(content);
            }
            catch (java.lang.NumberFormatException e){
                JOptionPane.showMessageDialog(null,"更新策略失败，参数格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            if(strategyCatalog.getStrategies().containsKey(bookType)){
                FlatRateStrategy flatRateStrategy=(FlatRateStrategy) strategyCatalog.getStrategies().get(bookType);
                if(flatRateStrategy.getStrategyNumber()!=strategyNumber&&strategyCatalog.hasStrategy(strategyNumber))
                    return false;
                flatRateStrategy.setStrategyNumber(strategyNumber);
                flatRateStrategy.setDiscountPerBook(discountPerBook);
                return true;
            }
            else
                return false;
        }
        else if(strategyType==StrategyType.PERCENTAGE){
            double discountPercentage;
            try {
                discountPercentage=Double.parseDouble(content);
            }
            catch (java.lang.NumberFormatException e){
                JOptionPane.showMessageDialog(null,"策略失败，参数格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                return false;
            }
            discountPercentage/=100;
            if(strategyCatalog.getStrategies().containsKey(bookType)){
                PercentageStrategy percentageStrategy=(PercentageStrategy) strategyCatalog.getStrategies().get(bookType);
                if(percentageStrategy.getStrategyNumber()!=strategyNumber&&strategyCatalog.hasStrategy(strategyNumber))
                    return false;
                percentageStrategy.setStrategyNumber(strategyNumber);
                percentageStrategy.setDiscountPercentage(discountPercentage);
                return true;
            }
            else
                return false;
        }
        else if(strategyType==StrategyType.COMPOSITE){
            if(strategyCatalog.getStrategies().containsKey(bookType)){
                IPricingStrategy strategy=strategyCatalog.getStrategies().get(bookType);
                int number=strategy.getStrategyNumber();
                if(number!=strategyNumber&&strategyCatalog.hasStrategy(strategyNumber))
                    return false;
                deleteStrategy(number);
                if(!addCompositeStrategy(bookType,strategyNumber,content)){
                    strategyCatalog.getStrategies().put(bookType,strategy);
                    return false;
                }
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    public boolean buyBook(int copies,String isbn){
        BookSpecification bookSpecification=bookCatalog.getBookSpecification(isbn);
        if(bookSpecification==null)
            return false;
        IPricingStrategy strategy=pricingStrategyFactory.getPricingStrategy(bookSpecification.getType());
        SaleLineItem saleLineItem=new SaleLineItem(copies,bookSpecification,strategy);
        sale.addSaleLineItem(saleLineItem);
        sale.notifyOberservers();
        return true;
    }
    public Sale getSale() {
        return sale;
    }
    public BookSpecification getBook(String isbn){
        return bookCatalog.getBookSpecification(isbn);
    }
    public IPricingStrategy returnStrategies(int strategyNumber){
        return strategyCatalog.returnStrategies(strategyNumber);
    }
    public BookCatalog getBookCatalog() {
        return bookCatalog;
    }
    public StrategyCatalog getStrategyCatalog() {
        return strategyCatalog;
    }
    public Controller(){
        bookCatalog=new BookCatalog();
        strategyCatalog=new StrategyCatalog();
        sale=new Sale();
        pricingStrategyFactory=new PricingStrategyFactory();
        observerForMains=new ArrayList<>();
        pricingStrategyFactory.setCatalog(strategyCatalog);
    }
}
