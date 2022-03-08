package matrix;

import java.util.Arrays;
import java.util.Scanner;
import java.util.stream.DoubleStream;

public class MatrixProduct {
    public static final Scanner IN = new Scanner(System.in);

    public static void main(String[] args) {
        while (true) {
            System.out.println("1. Multiplication");
            System.out.println("2. Line Multiplication");
            System.out.println("3. Block Multiplication");
            System.out.println("0. Quit");

            System.out.print("Selection:");
            int op = IN.nextInt();

            if (op == 0)
                return;

            System.out.print("Dimension (side):");
            int dim = IN.nextInt();

            switch (op) {
                case 1 -> mult(dim);
                case 2 -> multLine(dim);
                case 3 -> multBlock(dim);
                default -> System.out.printf("Invalid operation: %d", op);
            }
        }
    }

    private static void mult(int dim) {
        double[] ma = allocate(dim, DoubleStream.generate(() -> 1));
        double[] mb = allocate(dim, DoubleStream.iterate(1, i -> i + 1));

//        printMatrix(ma, dim);
//        printMatrix(mb, dim);

        double[] mc = new double[dim * dim];

        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                int temp = 0;
                for (int k = 0; k < dim; k++) {
                    temp += ma[i * dim + k] * mb[k * dim + j];
                }
                mc[i * dim + j] = temp;
            }
        }

        printMatrix(mc, dim);
    }

    private static void printMatrix(double[] m, int dim) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                System.out.printf("%.1f ", m[i * dim + j]);
            }
            System.out.println();
        }
        System.out.println();
    }

    private static double[] allocate(int dim, DoubleStream nums) {
        return nums.limit((long) dim * dim).toArray();
    }

    private static void multBlock(int dim) {

    }

    private static void multLine(int dim) {

    }
}