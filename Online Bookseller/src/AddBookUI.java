import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

public class AddBookUI extends JFrame implements ActionListener{
    private Controller controller;
    private JPanel panel;
    private JTextField isbn=new JTextField();
    private JTextField title=new JTextField();
    private JTextField price=new JTextField();
    private JComboBox type=new JComboBox();
    private JButton OK=new JButton("确定");
    public void actionPerformed(ActionEvent e){
        if(e.getSource()==OK){
            if(price.getText().isEmpty()||isbn.getText().isEmpty()||title.getText().isEmpty())
                JOptionPane.showMessageDialog(null,"请填写完整信息！","警告",JOptionPane.WARNING_MESSAGE);
            else {
                double Price;
                String Title=title.getText();
                String Isbn=isbn.getText();
                BookType bookType;
                try{
                    Price=Double.parseDouble(price.getText());
                }
                catch (java.lang.NumberFormatException e1){
                    JOptionPane.showMessageDialog(null,"信息格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                    return;
                }
                switch (type.getSelectedIndex()){
                    case 0:
                        bookType=BookType.NON_TEXTCOMPUTER;
                        break;
                    case 1:
                        bookType=BookType.TEXTBOOK;
                        break;
                    case 2:
                        bookType=BookType.COMICS;
                        break;
                    case 3:
                        bookType=BookType.HEALTHCARE;
                        break;
                    default:
                        bookType=BookType.OTHERS;
                }
                controller.addBook(new BookSpecification(Isbn,Title,Price,bookType));
                setVisible(false);
            }
        }
    }
    public AddBookUI(Controller controller){
        this.controller=controller;
        panel=new JPanel(new GridLayout(5,2));
        panel.add(new JLabel("ISBN："));
        panel.add(isbn);
        panel.add(new JLabel("书名："));
        panel.add(title);
        panel.add(new JLabel("价格："));
        panel.add(price);
        panel.add(new JLabel("类型："));
        type.addItem("非教材类计算机");
        type.addItem("教材");
        type.addItem("连环画");
        type.addItem("养生");
        type.addItem("其他");
        panel.add(type);
        panel.add(new JLabel());
        panel.add(OK);
        pack();
        price.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                int ch=e.getKeyChar();
                if(ch>=KeyEvent.VK_0&&ch<=KeyEvent.VK_9||ch==KeyEvent.VK_PERIOD){
                }
                else
                    e.consume();
            }
        });
        isbn.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                int ch=e.getKeyChar();
                if(ch>=KeyEvent.VK_0&&ch<=KeyEvent.VK_9||ch==KeyEvent.VK_MINUS){
                }
                else
                    e.consume();
            }
        });
        OK.addActionListener(this);
        add(panel);
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("添加书籍");
        setResizable(false);
        setSize(400,250);
        setLocation(500,100);
        setVisible(true);
    }
}
