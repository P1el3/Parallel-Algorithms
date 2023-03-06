import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicInteger;
import java.io.IOException;

public class Orders implements Runnable {
	int id;
	int P;
	String fileName;
	ExecutorService tpe;
	AtomicInteger curent_line;
	public Orders(int id, int P, String files, ExecutorService tpe, AtomicInteger curent_line) {
		this.id = id;
		this.P = P;
		this.fileName = files;
		this.tpe = tpe;
		this.curent_line = curent_line;
	}

	@Override
	public void run() {
		int local_line = 0;
		//read from file
		String path_order = fileName + "/orders.txt";
		File file = new File(path_order);
		FileReader fr = null;
		try {
			fr = new FileReader(file);
		} catch (FileNotFoundException e) {
			throw new RuntimeException(e);
		}
		BufferedReader br = new BufferedReader(fr);
		String line;

		while (true) {
			try {
				if ((line = br.readLine()) == null) {
					return;
				}
			} catch (IOException e) {
				throw new RuntimeException(e);
			}

			//process the line
			if(local_line % P == id){ //atribue each thread lines to work on
				String[] read = line.split(",");
				String no_order = read[0];

				int no_products = Integer.parseInt(read[1]);
				String path_product = fileName + "/order_products.txt";
				if(no_products != 0){
					tpe.submit(new Products(path_product, tpe, no_order, no_products));
				}
			}
			local_line++;

			//System.out.println(no_products.get());


		}

	}
}
