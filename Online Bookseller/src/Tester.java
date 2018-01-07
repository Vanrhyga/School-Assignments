import javax.swing.*;
import java.awt.*;

public class Tester {
    public static void main(String[] args){
        Font font =new Font("华文新魏",Font.PLAIN,24);
        Color color=new Color(255,255,243);
        UIManager.put("Label.font",font);
        UIManager.put("Button.font",font);
        UIManager.put("TextField.font",font);
        UIManager.put("ComboBox.font",font);
        UIManager.put("TextArea.background", color);
        font =new Font("华文新魏",Font.PLAIN,22);
        UIManager.put("Table.font",font);
        MainUI mainUI=new MainUI();
    }
}
