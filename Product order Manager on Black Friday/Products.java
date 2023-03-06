import java.io.*;
import java.util.concurrent.ExecutorService;
import java.io.FileReader;

public class Products implements Runnable{
	String fileName;
	ExecutorService tpe;
	String no_order;
	int no_products;
	public Products(String files, ExecutorService tpe, String no_order, int no_products) {
		this.fileName = files;
		this.tpe = tpe;
		this.no_order = no_order;
		this.no_products = no_products;
	}

	@Override
	public void run(){
		//open a file to read from
		File file = new File(fileName);
		FileReader fr = null;
		try {
			fr = new FileReader(file);
		} catch (FileNotFoundException e) {
			throw new RuntimeException(e);
		}
		BufferedReader br = new BufferedReader(fr);
		String line;

		int no_of_products = no_products;// used to count if I found all of the orders

		MySynchronizer wr = MySynchronizer.getInstance(); // avoid overwriting in the file
		while(true){
			try {
				if ((line = br.readLine()) == null) break;
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
			String[] read = line.split(",");

			if(read[0].equals(no_order)){ // if I found the product, I mark it as shipped
				no_products--;
				wr.writeToFile(read[0] + "," +read[1] + "," + "shipped\n", "order_products_out.txt");
			}
		}
		// less likely to overwrite in the output file so I use filewriter
		FileWriter myWriter_order = null;
		try {
			myWriter_order = new FileWriter("orders_out.txt", true);
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
		// if I shipped all the products, I mark the order as shipped
		if(no_products == 0){
			try {
				myWriter_order.write(no_order + "," + no_of_products + "," + "shipped\n");
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}
		try {
			myWriter_order.close();
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}
}
