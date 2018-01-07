import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Vector;

public class MainUI extends JFrame implements ObserverForMain,ActionListener{
    private JPanel panel;
    private TableModel tableModel=new TableModel();
    private JTable table=new JTable(tableModel);
    private JScrollPane scrollPane=new JScrollPane(table);
    private Controller controller;
    private JButton addBook=new JButton("添加图书");
    private JButton strategy=new JButton("管理优惠策略");
    private JButton buy=new JButton("购买图书");
    ShoppingCartUI shoppingCartUI;
    @Override
    public void update() {
        tableModel.updateUI();
    }
    class TableModel extends DefaultTableModel {
        private Vector data;
        private Vector title;
        public void addBook(BookSpecification bookSpecification){
            ArrayList list=new ArrayList();
            list.add(bookSpecification.getIsbn());
            list.add(bookSpecification.getTitle());
            list.add(bookSpecification.getPrice());
            String s;
            switch (bookSpecification.getType()){
                case NON_TEXTCOMPUTER:
                    s="非教材类计算机";
                    break;
                case TEXTBOOK:
                    s="教材";
                    break;
                case COMICS:
                    s="连环画";
                    break;
                case HEALTHCARE:
                    s="养生";
                    break;
                default:
                    s="其他";
            }
            list.add(s);
            data.add(list);
            table.repaint();
            table.updateUI();
        }
        public TableModel(){
            data=new Vector();
            title=new Vector();
            title.add("ISBN");
            title.add("书名");
            title.add("价格");
            title.add("类型");
            setDataVector(data,title);
        }
        public void updateUI(){
            data.clear();
            for (BookSpecification bookSpecification:controller.getBookCatalog().getBooks())
                addBook(bookSpecification);
        }
        public String getISBN(int row){
            ArrayList list=(ArrayList)data.get(row);
            return (String)list.get(0);
        }
        @Override
        public int getColumnCount() {
            return title.size();
        }
        @Override
        public Object getValueAt(int row, int column) {
            ArrayList list=(ArrayList)data.get(row);
            return list.get(column);
        }
        @Override
        public boolean isCellEditable(int row, int column) {
            return false;
        }
    }
    public void actionPerformed(ActionEvent e){
        if(e.getSource()==addBook){
            AddBookUI addBookUI=new AddBookUI(controller);
            addBookUI.setVisible(true);
        }
        else if(e.getSource()==buy){
            if(table.getSelectedRow()==-1)
                JOptionPane.showMessageDialog(null,"请选中购买书籍！","警告",JOptionPane.WARNING_MESSAGE);
            else {
                BuyUI buyUI=new BuyUI(controller,tableModel.getISBN(table.getSelectedRow()));
                buyUI.setVisible(true);
                shoppingCartUI.setVisible(true);
            }
        }
        else if(e.getSource()==strategy){
            StrategiesUI strategiesUI=new StrategiesUI(controller);
            strategiesUI.setVisible(true);
        }
    }
    public void init(){
        controller.addBook(new BookSpecification("978-7-302-2","UML与模式应用",18,BookType.TEXTBOOK));
        controller.addBook(new BookSpecification("978-7-312-3","Java与模式",34,BookType.NON_TEXTCOMPUTER));
        controller.addBook(new BookSpecification("968-6-302-1","HeadFirst 设计模式",58,BookType.NON_TEXTCOMPUTER));
        controller.addBook(new BookSpecification("958-1-302-2","爱丽丝历险记",30,BookType.COMICS));
        controller.addBook(new BookSpecification("900-7-392-2","煲汤大全",20,BookType.HEALTHCARE));
        controller.addSimpleStrategy(BookType.TEXTBOOK,StrategyType.FLATRATE,1,String.valueOf(1));
        controller.addSimpleStrategy(BookType.COMICS,StrategyType.PERCENTAGE,2,String.valueOf(7));
        controller.addSimpleStrategy(BookType.NON_TEXTCOMPUTER,StrategyType.PERCENTAGE,3,String.valueOf(3));
        controller.addCompositeStrategy(BookType.HEALTHCARE,4,"1|3");
    }
    public MainUI(){
        panel=new JPanel();
        setTitle("线上书店");
        panel.add(scrollPane);
        addBook.addActionListener(this);
        panel.add(addBook);
        strategy.addActionListener(this);
        panel.add(strategy);
        buy.addActionListener(this);
        panel.add(buy);
        controller=new Controller();
        controller.registerObserver(this);
        if(JOptionPane.showConfirmDialog(null,"是否需要自动添加信息？","",JOptionPane.YES_NO_OPTION)==JOptionPane.YES_OPTION)
            init();
        shoppingCartUI=new ShoppingCartUI(controller.getSale());
        shoppingCartUI.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        shoppingCartUI.setVisible(false);
        add(panel);
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        table.setPreferredScrollableViewportSize(new Dimension(800,640));
        table.setRowHeight(24);
        table.getTableHeader().setFont(new Font("华文新魏", Font.PLAIN, 22));
        table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        setResizable(false);
        setSize(1000,800);
        setLocation(500,100);
        setVisible(true);
    }
}
