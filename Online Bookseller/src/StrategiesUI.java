import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Vector;

public class StrategiesUI extends JFrame implements ActionListener{
    private Controller controller;
    private JPanel panel;
    private TableModel tableModel=new TableModel();
    private JTable table=new JTable(tableModel);
    private JScrollPane scrollPane=new JScrollPane(table);
    private JButton addFlat=new JButton("添加绝对值优惠策略");
    private JButton addPerc=new JButton("添加百分比折扣策略");
    private JButton addComp=new JButton("添加组合策略");
    private JButton modify=new JButton("修改策略");
    private JButton delete=new JButton("删除策略");
    private JComboBox type=new JComboBox();
    private JDialog add;
    private JTextField number=new JTextField();
    private JLabel detail=new JLabel("策略内容");
    private JTextField content=new JTextField();
    private JButton OK=new JButton("确定");
    private JButton cancel=new JButton("取消");
    private JDialog dialogModify;
    private JTextField number1=new JTextField();
    private JLabel detail1=new JLabel("策略内容");
    private JTextField content1=new JTextField();
    private JButton OK1=new JButton("确定");
    private JButton cancel1=new JButton("取消");
    private StrategyType strategyType;
    private BookType bookType;
    public void actionPerformed(ActionEvent e){
        if(e.getSource()==addFlat){
            detail.setText("每本书减：");
            add.setTitle("添加绝对值优惠策略");
            add.setPreferredSize(new Dimension(450,200));
            content.setText("");
            number.setText("");
            strategyType=StrategyType.FLATRATE;
            add.pack();
            add.setVisible(true);
        }
        else if(e.getSource()==addPerc){
            detail.setText("百分比折扣：");
            add.setTitle("添加百分比折扣策略");
            add.setPreferredSize(new Dimension(450,200));
            content.setText("");
            number.setText("");
            strategyType=StrategyType.PERCENTAGE;
            add.pack();
            add.setVisible(true);
        }
        else if(e.getSource()==addComp){
            detail.setText("依次输入组合策略的编号，并以|分隔：");
            add.setTitle("添加组合策略");
            add.setPreferredSize(new Dimension(880,200));
            content.setText("");
            number.setText("");
            strategyType=StrategyType.COMPOSITE;
            add.pack();
            add.setVisible(true);
        }
        else if(e.getSource()==delete){
            if(table.getSelectedRow()==-1)
                JOptionPane.showMessageDialog(null,"请选中需删除的策略！","警告",JOptionPane.WARNING_MESSAGE);
            else {
                int selectedNumber=tableModel.getNumber(table.getSelectedRow());
                ArrayList<IPricingStrategy> list=controller.returnCompositeStrategies(selectedNumber);
                if(list!=null) {
                    for (IPricingStrategy strategy : list) {
                        CompositeStrategy item=(CompositeStrategy) strategy;
                        ArrayList<IPricingStrategy> contents=item.getStrategies();
                        contents.remove(controller.getStrategyCatalog().returnStrategies(selectedNumber));
                        if(contents.size()==1){
                            BookType bookType1=controller.getStrategyCatalog().returnBookType(item.getStrategyNumber());
                            controller.deleteStrategy(item.getStrategyNumber());
                            if(contents.get(0).getStrategyType()==StrategyType.FLATRATE){
                                FlatRateStrategy flatRateStrategy=(FlatRateStrategy) contents.get(0);
                                FlatRateStrategy flatRateStrategy1=new FlatRateStrategy(flatRateStrategy.getStrategyNumber(),flatRateStrategy.getDiscountPerBook());
                                flatRateStrategy1.setStrategyNumber(item.getStrategyNumber());
                                controller.getStrategyCatalog().getStrategies().put(bookType1,flatRateStrategy1);
                            }
                            else{
                                PercentageStrategy percentageStrategy=(PercentageStrategy) contents.get(0);
                                PercentageStrategy percentageStrategy1=new PercentageStrategy(percentageStrategy.getStrategyNumber(),percentageStrategy.getDiscountPercentage());
                                percentageStrategy1.setStrategyNumber(item.getStrategyNumber());
                                controller.getStrategyCatalog().getStrategies().put(bookType1,percentageStrategy1);
                            }
                        }
                    }
                }
                controller.deleteStrategy(selectedNumber);
                JOptionPane.showMessageDialog(null,"删除成功！","",JOptionPane.INFORMATION_MESSAGE);
                tableModel.updateUI();
            }
        }
        else if(e.getSource()==cancel){
            number.setText("");
            content.setText("");
            add.setVisible(false);
        }
        else if(e.getSource()==cancel1){
            number1.setText("");
            content1.setText("");
            dialogModify.setVisible(false);
        }
        else if(e.getSource()==modify){
            if(table.getSelectedRow()==-1)
                JOptionPane.showMessageDialog(null,"请选中需修改的策略！","警告",JOptionPane.WARNING_MESSAGE);
            else {
                content1.setText("");
                int selectedNumber=tableModel.getNumber(table.getSelectedRow());
                strategyType=controller.getStrategyCatalog().returnStrategyType(selectedNumber);
                bookType=controller.getStrategyCatalog().returnBookType(selectedNumber);
                number1.setText(String.valueOf(selectedNumber));
                number1.setEditable(false);
                bookType=controller.getStrategyCatalog().returnBookType(selectedNumber);
                dialogModify.setVisible(true);
            }
        }
        else if(e.getSource()==OK){
            if(number.getText().isEmpty()||content.getText().isEmpty())
                JOptionPane.showMessageDialog(null,"请填写完整信息！","警告",JOptionPane.WARNING_MESSAGE);
            else {
                int tmp;
                try{
                    tmp=Integer.parseInt(number.getText());
                }
                catch (java.lang.NumberFormatException e1){
                    JOptionPane.showMessageDialog(null,"信息格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                    return;
                }
                switch (strategyType){
                    case FLATRATE:
                        try {
                            Integer.parseInt(content.getText());
                        }
                        catch (java.lang.NumberFormatException e1){
                            JOptionPane.showMessageDialog(null,"信息格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if (controller.addSimpleStrategy(returnBookType(type.getSelectedIndex()), strategyType, tmp, content.getText())) {
                            JOptionPane.showMessageDialog(null,"添加成功！","",JOptionPane.INFORMATION_MESSAGE);
                            content.setText("");
                            number.setText("");
                            add.setVisible(false);
                        }
                        else
                            return;
                        break;
                    case PERCENTAGE:
                        double discount;
                        try {
                            discount=Double.parseDouble(content.getText());
                        }
                        catch (java.lang.NumberFormatException e1){
                            JOptionPane.showMessageDialog(null,"信息格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if(discount>100){
                            JOptionPane.showMessageDialog(null,"折扣信息有误！","错误",JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if(controller.addSimpleStrategy(returnBookType(type.getSelectedIndex()),strategyType,tmp,content.getText())){
                            JOptionPane.showMessageDialog(null,"添加成功！","",JOptionPane.INFORMATION_MESSAGE);
                            content.setText("");
                            number.setText("");
                            add.setVisible(false);
                        }
                        else
                            return;
                        break;
                    case COMPOSITE:
                        if (controller.addCompositeStrategy(returnBookType(type.getSelectedIndex()), tmp, content.getText())) {
                            JOptionPane.showMessageDialog(null,"添加成功！","",JOptionPane.INFORMATION_MESSAGE);
                            content.setText("");
                            number.setText("");
                            add.setVisible(false);
                        }
                        else
                            return;
                        break;
                    default:
                        return;
                }
                tableModel.updateUI();
                number.setText("");
                content.setText("");
            }
        }
        else if(e.getSource()==OK1){
            if(content1.getText().isEmpty())
                JOptionPane.showMessageDialog(null,"请填写完整信息！","警告",JOptionPane.WARNING_MESSAGE);
            else {
                switch (strategyType){
                    case FLATRATE:
                        try {
                            Integer.parseInt(content1.getText());
                        }
                        catch (java.lang.NumberFormatException e1){
                            JOptionPane.showMessageDialog(null,"信息格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if (controller.updateStrategy(bookType, strategyType, Integer.parseInt(number1.getText()), content1.getText())) {
                            JOptionPane.showMessageDialog(null,"更改成功！","",JOptionPane.INFORMATION_MESSAGE);
                            content1.setText("");
                            number1.setText("");
                            dialogModify.setVisible(false);
                        }
                        else
                            return;
                        break;
                    case PERCENTAGE:
                        double discount;
                        try {
                            discount=Double.parseDouble(content1.getText());
                        }
                        catch (java.lang.NumberFormatException e1){
                            JOptionPane.showMessageDialog(null,"信息格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if(discount>=100){
                            JOptionPane.showMessageDialog(null,"折扣信息有误！","错误",JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if(controller.updateStrategy(bookType,strategyType,Integer.parseInt(number1.getText()),content1.getText())){
                            JOptionPane.showMessageDialog(null,"更改成功！","",JOptionPane.INFORMATION_MESSAGE);
                            content1.setText("");
                            number1.setText("");
                            dialogModify.setVisible(false);
                        }
                        else
                            return;
                        break;
                    case COMPOSITE:
                        if (controller.updateStrategy(bookType,strategyType, Integer.parseInt(number1.getText()), content1.getText())) {
                            JOptionPane.showMessageDialog(null,"更改成功！","",JOptionPane.INFORMATION_MESSAGE);
                            content1.setText("");
                            number1.setText("");
                            dialogModify.setVisible(false);
                        }
                        else
                            return;
                        break;
                    default:
                        return;
                }
                tableModel.updateUI();
                number1.setText("");
                content1.setText("");
            }
        }
    }
    class TableModel extends DefaultTableModel {
        private Vector data;
        private Vector title;
        String s;
        public void addItem(BookType bookType,IPricingStrategy strategy){
            ArrayList list=new ArrayList();
            list.add(strategy.getStrategyNumber());
            if(strategy.getStrategyType()==StrategyType.FLATRATE){
                list.add("绝对值优惠");
            }
            else if(strategy.getStrategyType()==StrategyType.PERCENTAGE){
                list.add("百分比折扣");
            }
            else if(strategy.getStrategyType()==StrategyType.COMPOSITE){
                list.add("组合");
            }
            else{
                list.add("无");
            }
            list.add(strategy.toString());
            s="";
            switch (bookType){
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
            title.add("策略编号");
            title.add("策略类型");
            title.add("策略内容");
            title.add("图书类型");
            setDataVector(data,title);
        }
        public void updateUI(){
            data.clear();
            for(HashMap.Entry<BookType,IPricingStrategy> entry:controller.getStrategyCatalog().getStrategies().entrySet())
                addItem(entry.getKey(),entry.getValue());
        }
        public int getNumber(int row){
            ArrayList list=(ArrayList) data.get(row);
            return (int)list.get(0);
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
    public BookType returnBookType(int number){
        switch (number){
            case 0:
                return BookType.NON_TEXTCOMPUTER;
            case 1:
                return BookType.TEXTBOOK;
            case 2:
                return BookType.COMICS;
            case 3:
                return BookType.HEALTHCARE;
            default:
                return BookType.OTHERS;
        }
    }
    public StrategiesUI(Controller controller){
        this.controller=controller;
        panel=new JPanel();
        setTitle("策略管理");
        panel.add(scrollPane);
        addFlat.addActionListener(this);
        panel.add(addFlat);
        addPerc.addActionListener(this);
        panel.add(addPerc);
        addComp.addActionListener(this);
        panel.add(addComp);
        modify.addActionListener(this);
        panel.add(modify);
        delete.addActionListener(this);
        panel.add(delete);
        type.addItem("非教材类计算机");
        type.addItem("教材");
        type.addItem("连环画");
        type.addItem("养生");
        type.addItem("其他");
        add=new JDialog();
        add.setLayout(new GridLayout(4,2));
        add.add(new JLabel("策略编号："));
        add.add(number);
        number.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                int ch=e.getKeyChar();
                if(ch<KeyEvent.VK_0||ch>KeyEvent.VK_9){
                    e.consume();
                }
            }
        });
        add.add(detail);
        add.add(content);
        add.add(new JLabel("图书类型："));
        add.add(type);
        OK.addActionListener(this);
        add.add(OK);
        cancel.addActionListener(this);
        add.add(cancel);
        add.setLocation(500,100);
        add.setModal(true);
        dialogModify=new JDialog();
        dialogModify.setLayout(new GridLayout(3,2));
        dialogModify.add(new JLabel("策略编号："));
        dialogModify.add(number1);
        number1.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                int ch=e.getKeyChar();
                if(ch<KeyEvent.VK_0||ch>KeyEvent.VK_9){
                    e.consume();
                }
            }
        });
        dialogModify.add(detail1);
        dialogModify.add(content1);
        OK1.addActionListener(this);
        dialogModify.add(OK1);
        cancel1.addActionListener(this);
        dialogModify.add(cancel1);
        dialogModify.setPreferredSize(new Dimension(300,200));
        dialogModify.setLocation(500,100);
        dialogModify.setModal(true);
        dialogModify.pack();
        tableModel.updateUI();
        add(panel);
        pack();
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        table.setPreferredScrollableViewportSize(new Dimension(800,640));
        table.setRowHeight(24);
        table.getTableHeader().setFont(new Font("华文新魏", Font.PLAIN, 22));
        table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        setSize(1000,800);
        setResizable(false);
        setLocation(500,100);
    }
}
