package com.egls.test;

/**
 * 
[0]1.0,[1]2.0,:[0]0.9800804466483455,[1]0.02001329327015804,
[0]2.0,[1]2.0,:[0]0.018657706537032644,[1]0.9811385715605158,
[0]1.0,[1]1.0,:[0]0.016855945723243174,[1]0.9833300479959753,
[0]2.0,[1]1.0,:[0]0.9830225443619623,[1]0.01697611899730402,
[0]3.0,[1]1.0,:[0]0.998950833781844,[1]0.0011017165802983588,
 * @author gust
 */
public class BpDeepTest {

    static String arr2str(double[] arr) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < arr.length; i++) {
            sb.append("[").append(i).append("]").append(arr[i]).append(',');
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        //????????????
        //???????????????????????????????{3,10,10,10,10,2}??????3????????2???????4???????10???
        //?????????????????????
        BpDeep bp = new BpDeep(new int[]{2, 10, 2}, 0.15, 0.8);

        //????????????4???????
        double[][] data = new double[][]{{1, 2}, {2, 2}, {1, 1}, {2, 1}};
        //?????????4????????
        double[][] target = new double[][]{{1, 0}, {0, 1}, {0, 1}, {1, 0}};

        //????5000?
        for (int n = 0; n < 5000; n++) {
            for (int i = 0; i < data.length; i++) {
                bp.train(data[i], target[i]);
            }
        }

        //?????????????
        for (int j = 0; j < data.length; j++) {
            double[] result = bp.computeOut(data[j]);
            System.out.println(arr2str(data[j]) + ":" + arr2str(result));
        }

        //?????????????????
        double[] x = new double[]{3, 1};
        double[] result = bp.computeOut(x);
        System.out.println(arr2str(x) + ":" + arr2str(result));
    }
}
