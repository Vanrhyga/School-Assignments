import java.util.ArrayList;

public class Sale implements Subject{
    private ArrayList<SaleLineItem> items;
    private ArrayList<Observer> observers;
    @Override
    public void removeObserver(Observer observer){
        if(observers.contains(observer))
            observers.remove(observer);
    }
    @Override
    public void registerObserver(Observer observer){
        observers.add(observer);
    }
    @Override
    public void notifyOberservers() {
        for (Observer observer:observers)
            observer.update(this);
    }
    public double getTotal(){
        double total=0;
        double tmp=0;
        for (SaleLineItem saleLineItem:items){
            tmp=saleLineItem.getSubTotal();
            total+=tmp;
        }
        return total;
    }
    public void addSaleLineItem(SaleLineItem item){
        items.add(item);
    }
    public ArrayList<SaleLineItem> getItems() {
        return items;
    }
    public Sale(){
        items=new ArrayList<>();
        observers=new ArrayList<>();
    }
}
