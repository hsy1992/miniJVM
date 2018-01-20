package test;

import org.mini.gui.GFrame;
import org.mini.gui.GForm;
import static org.mini.gui.GToolkit.getForm;
import static org.mini.nk.NK.nk_button_label;
import static org.mini.nk.NK.nk_false;
import static org.mini.nk.NK.nk_layout_row_dynamic;
import static org.mini.nk.NK.nk_layout_row_static;
import static org.mini.nk.NK.nk_option_label;
import static org.mini.nk.NK.nk_propertyi;
import static org.mini.nk.NK.nk_true;
import static org.mini.nk.NK.nk_window_find;
import static org.mini.nk.NK.nk_window_show;
import org.mini.gui.GFrameContents;

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

    class NkFormContents1 implements GFrameContents {

        final int EASY = 0, MID = 1, HARD = 2;
        int op = EASY;
        int property = 20;
        String sub_form_name = "Notice";

        @Override
        public void updateContents(long ctx, GFrame parent) {

            nk_layout_row_static(ctx, 30f, 80, 1);
            if (nk_true==nk_button_label(ctx, "button")) {
                System.out.println("button pressed\n");
                long notice = nk_window_find(ctx, sub_form_name);
                if (notice == 0) {
                    GForm main = getForm(ctx);
                    System.out.println("main window:" + main);
                    main.add(new GFrame(sub_form_name, 100, 100, 300, 400, new GFrameContents() {
                        @Override
                        public void updateContents(long ctx, GFrame parent) {
                            nk_layout_row_static(ctx, 30, 80, 1);
                            if (nk_true==nk_button_label(ctx, "Ok")) {
                                System.out.println("Ok pressed\n");
                            }
                        }
                    }));
                } else {
                    nk_window_show(ctx, sub_form_name,1);
                }
            }
            nk_layout_row_dynamic(ctx, 30, 3);
            if (nk_option_label(ctx, "easy", op == EASY?nk_true:nk_false)==nk_true) {
                op = EASY;
            }
            if (nk_option_label(ctx, "mid", op == MID?nk_true:nk_false)==nk_true) {
                op = MID;
            }
            if (nk_option_label(ctx, "hard", op == HARD?nk_true:nk_false)==nk_true) {
                op = HARD;
            }
            nk_layout_row_dynamic(ctx, 22, 1);
            property = nk_propertyi(ctx, "Compression:", 0, property, 100, 10, 1);

        }

    }

    void t1() {
        GForm win = new GForm("test", 800, 600);
        win.add(new GFrame("demo", 30, 30, 300, 200, new NkFormContents1()));
        //NkForm win1 = new NkForm("t1", 500, 300);
    }

    public static void main(String[] args) {
        GuiTest gt = new GuiTest();
        gt.t1();

    }
}
