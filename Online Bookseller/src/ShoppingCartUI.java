import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Vector;

public class ShoppingCartUI extends JFrame implements Observer,ActionListener{
    private Subject subject;
    private TableModel tableModel=new TableModel();
    private JTable table=new JTable(tableModel);
    private JScrollPane scrollPane=new JScrollPane(table);
    private JPanel panel=new JPanel();
    private JLabel label=new JLabel();
    @Override
    public void update(Sale sale) {
        tableModel.updateUI(sale);
        label.setText("总价：共"+String.format("%.2f元",sale.getTotal()));
    }
    public void actionPerformed(ActionEvent e){
    }
    class TableModel extends DefaultTableModel{
        private Vector data;
        private Vector title;
        public void addSaleLineItem(SaleLineItem saleLineItem){
            ArrayList list=new ArrayList();
            list.add(saleLineItem.getProdSpec().getTitle());
            if(saleLineItem.getProdSpec().getType()==BookType.NON_TEXTCOMPUTER)
                list.add("非教材类计算机");
            else if(saleLineItem.getProdSpec().getType()==BookType.TEXTBOOK)
                list.add("教材");
            else if(saleLineItem.getProdSpec().getType()==BookType.COMICS)
                list.add("连环画");
            else if(saleLineItem.getProdSpec().getType()==BookType.HEALTHCARE)
                list.add("养生");
            else
                list.add("其他");
            list.add(saleLineItem.getProdSpec().getPrice());
            list.add(saleLineItem.getStrategy());
            list.add(saleLineItem.getCopies());
            data.add(list);
        }
        public TableModel(){
            data=new Vector();
            title=new Vector();
            title.add("书名");
            title.add("类型");
            title.add("单价");
            title.add("优惠策略");
            title.add("购买数量");
            setDataVector(data,title);
        }
        public void updateUI(Sale sale){
            data.clear();
            for (SaleLineItem saleLineItem:sale.getItems())
                addSaleLineItem(saleLineItem);
            table.repaint();
            table.updateUI();
        }
        @Override
        public int getColumnCount() {
            return title.size();
        }
        @Override
        public Object getValueAt(int row, int column) {
            ArrayList list=(ArrayList)this.data.get(row);
            return list.get(column);
        }
        @Override
        public boolean isCellEditable(int row, int column) {
            return false;
        }
    }
    public ShoppingCartUI(Subject subject){
        this.subject=subject;
        this.subject.registerObserver(this);
        panel.add(scrollPane);
        label.setText("总价：共"+String.valueOf(0)+"元");
        panel.add(label);
        add(panel);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        table.setPreferredScrollableViewportSize(new Dimension(800,640));
        table.setRowHeight(24);
        table.getTableHeader().setFont(new Font("华文新魏", Font.PLAIN, 22));
        table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        setTitle("购物车");
        setResizable(false);
        setSize(1000,800);
        setLocation(500,100);
        setVisible(true);
    }
}
