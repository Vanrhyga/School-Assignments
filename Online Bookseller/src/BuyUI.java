import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

public class BuyUI extends JFrame implements ActionListener{
    private Controller controller;
    private BookSpecification bookSpecification;
    private JPanel panel;
    private JLabel title=new JLabel();
    private JTextField sum=new JTextField();
    private JButton OK=new JButton("确定");
    public void actionPerformed(ActionEvent e){
        if(e.getSource()==OK){
            int copies;
            if(sum.getText().isEmpty()){
                JOptionPane.showMessageDialog(null,"请填写完整信息！","警告",JOptionPane.WARNING_MESSAGE);
                return;
            }
            else {
                try {
                    copies=Integer.parseInt(sum.getText());
                }
                catch (java.lang.NumberFormatException e1){
                    JOptionPane.showMessageDialog(null,"信息格式有误！","错误",JOptionPane.ERROR_MESSAGE);
                    return;
                }
                if(controller.buyBook(copies,bookSpecification.getIsbn())){
                    JOptionPane.showMessageDialog(null,"加入购物车成功！","",JOptionPane.INFORMATION_MESSAGE);
                    setVisible(false);
                    return;
                }
                else {
                    JOptionPane.showMessageDialog(null,"加入购物车失败，请重试！","错误",JOptionPane.ERROR_MESSAGE);
                    return;
                }
            }
        }
    }
    public BuyUI(Controller controller,String isbn){
        this.controller=controller;
        panel=new JPanel(new GridLayout(3,2));
        bookSpecification=controller.getBook(isbn);
        setTitle("购买书籍");
        panel.add(new JLabel("书名："));
        title.setText(bookSpecification.getTitle());
        panel.add(title);
        panel.add(new JLabel("数量："));
        sum.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                int ch=e.getKeyChar();
                if(ch>=KeyEvent.VK_0&&ch<=KeyEvent.VK_9){
                }
                else
                    e.consume();
            }
        });
        panel.add(sum);
        panel.add(new JLabel());
        panel.add(OK);
        OK.addActionListener(this);
        add(panel);
        pack();
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        setResizable(false);
        setSize(300,150);
        setLocation(200,100);
        setVisible(true);
    }
}
