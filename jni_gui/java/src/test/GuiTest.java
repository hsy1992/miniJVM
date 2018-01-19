package test;

import org.mini.gui.GWindow;
import org.mini.gui.GForm;
import static org.mini.gui.GToolkit.getForm;
import org.mini.gui.GWindowContents;
import static org.mini.nk.NK.nk_button_label;
import static org.mini.nk.NK.nk_false;
import static org.mini.nk.NK.nk_layout_row_dynamic;
import static org.mini.nk.NK.nk_layout_row_static;
import static org.mini.nk.NK.nk_option_label;
import static org.mini.nk.NK.nk_propertyi;
import static org.mini.nk.NK.nk_true;
import static org.mini.nk.NK.nk_window_find;
import static org.mini.nk.NK.nk_window_show;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author gust
 */
public class GuiTest {

    class NkFormContents1 implements GWindowContents {

        final int EASY = 0, MID = 1, HARD = 2;
        int op = EASY;
        int property = 20;
        String sub_form_name = "Notice";

        @Override
        public void updateContents(long win_ptr, GWindow parent) {

            nk_layout_row_static(win_ptr, 30f, 80, 1);
            if (nk_true==nk_button_label(win_ptr, "button")) {
                System.out.println("button pressed\n");
                long notice = nk_window_find(win_ptr, sub_form_name);
                if (notice == 0) {
                    GForm main = getForm(win_ptr);
                    System.out.println("main window:" + main);
                    main.add(new GWindow(sub_form_name, 100, 100, 300, 400, new GWindowContents() {
                        @Override
                        public void updateContents(long win_ptr, GWindow parent) {
                            nk_layout_row_static(win_ptr, 30, 80, 1);
                            if (nk_true==nk_button_label(win_ptr, "Ok")) {
                                System.out.println("Ok pressed\n");
                            }
                        }
                    }));
                } else {
                    nk_window_show(win_ptr, sub_form_name,1);
                }
            }
            nk_layout_row_dynamic(win_ptr, 30, 3);
            if (nk_option_label(win_ptr, "easy", op == EASY?nk_true:nk_false)==nk_true) {
                op = EASY;
            }
            if (nk_option_label(win_ptr, "mid", op == MID?nk_true:nk_false)==nk_true) {
                op = MID;
            }
            if (nk_option_label(win_ptr, "hard", op == HARD?nk_true:nk_false)==nk_true) {
                op = HARD;
            }
            nk_layout_row_dynamic(win_ptr, 22, 1);
            property = nk_propertyi(win_ptr, "Compression:", 0, property, 100, 10, 1);

        }

    }

    void t1() {
        GForm win = new GForm("test", 800, 600);
        win.add(new GWindow("demo", 30, 30, 300, 200, new NkFormContents1()));
        //NkForm win1 = new NkForm("t1", 500, 300);
    }

    public static void main(String[] args) {
        GuiTest gt = new GuiTest();
        gt.t1();

    }
}
