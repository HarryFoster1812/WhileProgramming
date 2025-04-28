import java.util.Scanner;
import java.lang.Math;

public class Main {
    private static Scanner sc = new Scanner(System.in);

    public static void main(String[] args) {
        int input;
        do {
            input = getValue();

            String result = inverse_phi_s(input);
            System.out.println(result + "\n\n");
        } while (input >= 0);
        sc.close();

    }

    public static int getValue() {
        System.out.println("Program Number: ");
        int input = sc.nextInt();
        return input;
    }

    public static String inverse_phi_s(int program) {
        return inverse_phi_s(program, 0);
    }

    public static String inverse_phi_s(int program, int indentation) {
        if (program == 0) {
            return " ".repeat(3 * indentation) + "skip\n";
        }
        int[] values = new int[2];

        return " ".repeat(3 * indentation) + switch (program % 4) {
            case 0:
                program /= 4;
                program--;
                values = inverse_phi(program);
                yield "while " + inverse_phi_b(values[0]) + " do \n" + inverse_phi_s(values[1], indentation + 1);
            case 1:
                program -= 1;
                program /= 4;
                values = inverse_phi(program);

                yield inverse_phi_v(values[0]) + " := " + inverse_phi_a(values[1]) + "\n";
            case 2:
                program -= 2;
                program /= 4;
                values = inverse_phi(program);

                yield inverse_phi_s(values[0]) + ";" + inverse_phi_s(values[1]);
            case 3:
                program -= 3;
                program /= 4;
                values = inverse_phi(program);
                int[] statements = inverse_phi(values[1]);
                yield "if " + inverse_phi_b(values[0]) + " then \n" + inverse_phi_s(statements[0], indentation + 1)
                        + " else\n"
                        + inverse_phi_s(statements[1], indentation + 1) + "";
            default:
                yield "";

        };

    }

    public static String inverse_phi_v(int program) {
        return "x_{" + Integer.toString(program) + "}";
    }

    public static String inverse_phi_b(int program) {
        if (program == 0) {
            return "\\false";
        } else if (program == 1) {
            return "\\true";
        }
        int[] values = new int[2];
        return switch (program % 4) {
            case 0:
                program -= 4;
                program /= 4;
                yield "\\lnot " + inverse_phi_b(program);
            case 1:
                program -= 5;
                program /= 4;
                values = inverse_phi(program);

                yield inverse_phi_b(values[0]) + " \\land " + inverse_phi_b(values[1]);
            case 2:
                program -= 2;
                program /= 4;
                values = inverse_phi(program);

                yield inverse_phi_a(values[0]) + " = " + inverse_phi_a(values[1]);
            case 3:
                program -= 3;
                program /= 4;
                values = inverse_phi(program);

                yield inverse_phi_a(values[0]) + " \\leq " + inverse_phi_a(values[1]);
            default:
                yield "";
        };

    }

    public static String inverse_phi_a(int program) {
        int[] values = new int[2];
        return switch (program % 5) {
            case 0:
                Integer.toString(program / 5);
            case 1:
                program--;
                program /= 5;

                yield inverse_phi_v(program);
            case 2:
                program -= 2;
                program /= 5;
                values = inverse_phi(program);

                yield inverse_phi_a(values[0]) + " + " + inverse_phi_a(values[1]);
            case 3:
                program -= 3;
                program /= 5;
                values = inverse_phi(program);

                yield inverse_phi_a(values[0]) + " - " + inverse_phi_a(values[1]);
            case 4:
                program -= 4;
                program /= 5;
                values = inverse_phi(program);

                yield inverse_phi_a(values[0]) + " \\times " + inverse_phi_a(values[1]);
            default:
                yield "";

        };

    }

    public static int[] inverse_phi(int value) {
        value += 1;
        int m = 0;
        while (value % 2 == 0) {
            m += 1;
            value /= 2;
        }
        int n = (value - 1) / 2;
        return new int[] { m, n };
    }
}
