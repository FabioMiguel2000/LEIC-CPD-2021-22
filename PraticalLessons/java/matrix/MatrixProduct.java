package matrix;

import java.util.Arrays;
import java.util.Collections;
import java.util.Optional;
import java.util.Scanner;
import java.util.stream.IntStream;

enum Operation {
    NORMAL(1, "Normal Multiplication"),
    LINE(2, "Line Multiplication"),
    BLOCK(3, "Block Multiplication"),
    EXIT(0, "Exit");


    final String desc;
    final int num;

    Operation(int i, String s) {
        num = i;
        desc = s;
    }

    public static Operation from(String s) {
        return switch (s.trim().toLowerCase()) {
            case "0", "exit" -> EXIT;
            case "2", "line" -> LINE;
            case "3", "block" -> BLOCK;
            default -> NORMAL;
        };
    }

    @Override
    public String toString() {
        return desc;
    }
}

public class MatrixProduct {
    public static final Scanner IN = new Scanner(System.in);

    public static void main(String[] args) {
        while (true) {
            Arrays.stream(Operation.values()).forEach(op -> System.out.printf("%d. %s\n", op.num, op));

            System.out.print("Selection:");
            Operation op = Operation.from(IN.next());
            if (op == Operation.EXIT)
                return;

            System.out.printf("Using %s\n", op);

            System.out.print("Dimension (side):");
            int dim = IN.nextInt();

            switch (op) {
                case NORMAL -> mult(dim);
                case LINE -> multLine(dim);
                case BLOCK -> multBlock(dim);
            }
        }
    }

    private static void mult(int dim) {
        double[] ma = Collections.nCopies(dim * dim, 1).stream().mapToDouble(i -> i).toArray();
        double[] mb = IntStream.rangeClosed(1, dim)
                .mapToObj(i -> Collections.nCopies(dim, i).stream().mapToDouble(d -> d))
                .flatMapToDouble(s -> s)
                .toArray();


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

        printMatrix(mc, dim, 10);
//        printMatrix(mc, dim);
    }


    private static void multLine(int dim) {
        double[] ma = Collections
                .nCopies(dim * dim, 1)
                .stream()
                .mapToDouble(i -> i)
                .toArray();

        double[] mb = IntStream
                .rangeClosed(1, dim)
                .mapToObj(i -> Collections.nCopies(dim, i).stream().mapToDouble(d -> d))
                .flatMapToDouble(s -> s)
                .toArray();

//        printMatrix(ma, dim);
//        printMatrix(mb, dim);

        double[] mc = new double[dim * dim];

        for (int line = 0; line < dim; line++) {
            for (int col = 0; col < dim; col++) {
                for (int k = 0; k < dim; k++) {
                    mc[line * dim + k] += ma[line * dim + col] * mb[col * dim + k];
                }
            }
        }

        printMatrix(mc, dim);
    }

    private static void printMatrix(double[] m, int dim, int n) {
        m:
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++, n--) {
                System.out.printf("%.0f ", m[i * dim + j]);
                if (n <= 1)
                    break m;
            }
            System.out.println();
        }
        System.out.println();
    }

    private static void printMatrix(double[] m, int dim) {
        printMatrix(m, dim, dim * dim);
    }

    private static void multBlock(int dim) {
        System.out.print("Block size:");
        int blk = IN.nextInt();

        double[] ma = Collections
                .nCopies(dim * dim, 1)
                .stream()
                .mapToDouble(i -> i)
                .toArray();

        double[] mb = IntStream
                .rangeClosed(1, dim)
                .mapToObj(i -> Collections.nCopies(dim, i).stream().mapToDouble(d -> d))
                .flatMapToDouble(s -> s)
                .toArray();

        double[] mc = new double[dim * dim];

        for (int i = 0; i < dim / blk; i++) {
            for (int j = 0; j < dim / blk; j++) {
                for (int line = 0; line < blk; line++) {
                    for (int col = 0; col < blk; col++) {
                        for (int k = 0; k < dim; k++) {
                            int iLine = (i * blk + line) * dim;
                            int iCol = j * blk + col;
                            mc[iLine + k] += ma[iLine + iCol] * mb[iCol * dim + k];
                        }
                    }
                }
            }
        }

        printMatrix(mc, dim);
    }
}