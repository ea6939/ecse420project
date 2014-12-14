import java.awt.image.*;
import java.io.*;
import javax.imageio.ImageIO;

public class ImageWriter {
	static String outputFilename;

	public static void main(String[] args) throws IOException {
		if(args.length != 2) {
			System.out.println("Usage: java ImageWriter <input filename> <output image filename>");
			System.exit(0);
		}

		outputFilename = args[1];
		readFile(args[0]);
	}

	static void readFile(String filename) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(filename));
		try {
			int height, width;
			int r, g, b;

			String line = br.readLine();
			width = Integer.parseInt(line);
			line = br.readLine();
			height = Integer.parseInt(line);

			BufferedImage output = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

			for(int j = 0; j < height; j++) {
				for(int i = 0; i < width; i++) {
					line = br.readLine();
					r = Integer.parseInt(line.split(" ")[0]);
					g = Integer.parseInt(line.split(" ")[1]);
					b = Integer.parseInt(line.split(" ")[2]);

					output.setRGB(i, j, ((255 & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8)  | ((b & 0xFF) << 0));
				}
			}

			br.close();

			//Print out image
			File outputFile = new File(outputFilename);
			ImageIO.write(output, "bmp", outputFile);
		}
		catch(Exception e) { System.out.println("An error has occurred."); }
	}
}