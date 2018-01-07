public interface SubjectForMain {
    void removeObserver(ObserverForMain observer);
    void registerObserver(ObserverForMain observer);
    void notifyObservers();
}
