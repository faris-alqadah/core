require "rubygems"
require "commandline"
require "set"
require "ncluster"


class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2011"
    synopsis  "cluster_file output_file_base [--big_network][--all_network]"
    short_description "Read an n-cluster file produce by DMRC package and create different cytoscape files for visualization. Assumption is that clusters are star shaped."
    long_description "Read an n-cluster file produce by DMRC package and create different cytoscape files for visualization. Assumption is that clusters are star shaped."
    options :help

  

     option :flag,
      :names => "--big_network",
      :opt_found => get_args,
      :opt_not_found => false,
      :opt_description => "Makes one large network cytoscape file as combination of all clusters"

    option :flag,
      :names => "--all_networks",
      :opt_found => get_args,
      :opt_not_found => false,
      :opt_description => "Makes single cytoscape file per cluster"

    expected_args :cluster_file, :output_base_name

  end

def main
  begin
      clusters = Ncluster.read_clusters(@cluster_file)

  rescue => err
    puts err.to_s
    puts err.backtrace
  end

end

  ##
  #Makes a large network from a set of n-clusters.
  #Network is represented as hash where obj_id -> adj_list (array of objects)
  # Input:
  #     clusters: n-clusters as an array of hahses
  #
  # Output: none
  #
  # Returns:
  #     network: adjacency list in form of hash
  def make_network(clusters)
      adj_list = Hash.new
      for c in clusters  # hashes
        num_domains = c.keys.size
        a_nodes = c[1] # assumption of star shaped clusters
        # get b_nodes
        b_nodes=c[2]
        for i in 3..num_domains
          b_nodes = b_nodes + c[i]
        end
       

      end
  end



end