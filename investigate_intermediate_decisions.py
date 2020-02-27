import click
import pandas
import requests
import logging


def format_hand(hand):
    # reformat hand to solver api spec
    # "KdKh--AdAcAh" -> Kd Kh//Ad Ac Ah fml
    formatted_hand_rows = []
    for hand_row in hand.split("-"):
        cards = []
        for j in range(0, len(hand_row) // 2):
            cards.append(hand_row[2 * j:2 * j + 2])

        formatted_hand_rows.append(" ".join(cards))

    formatted_hand = "/".join(formatted_hand_rows)
    return formatted_hand


@click.command()
@click.option('--filename', default='sims.txt', help='file which contains list of all sims')
@click.option('--solver-ip', default='localhost', help='IP address of ofcsolver')
def main(filename, solver_ip):
    # requests_log = logging.getLogger("requests.packages.urllib3")
    # requests_log.setLevel(logging.DEBUG)
    # requests_log.propagate = True
    with open(filename, 'r') as f:
        for sim_file in f.readlines():
            print(sim_file)
            df = pandas.read_csv('data/' + sim_file.strip())

            for i in range(0, 3):
                # element is (decision: string, ev: float)
                resolved_decisions = []
                if not (f"Pull_{i}" in df):
                    break

                # sim hand_i with pull_i
                for row in df.itertuples():
                    # hand: "KdKh--AdAcAh"
                    # pull: "Jh Qh 4s"
                    hand = getattr(row, f"Hand_{i}").strip()
                    pull = getattr(row, f"Pull_{i}").strip()
                    formatted_hand = format_hand(hand)

                    # call ofcsolver
                    print("Hand: {}".format(formatted_hand))
                    print("Pull: {}".format(pull))

                    resp = requests.get(
                        "http://" + solver_ip + ":8080/eval",
                        params={
                            'type': 'ultimate',
                            'pull': pull,
                            'my_hand': formatted_hand
                        }
                    )

                    resolved_decision = resp.text.strip()
                    decision = getattr(row, f"Decision_{i}").strip()

                    # TODO: need to return ev, and check ev tolerance
                    # with the ev, i can somewhat judge how bad the mistake was
                    resolved_decisions.append(resolved_decision)

                df[f"ResolvedDecision_{i}"] = resolved_decisions

            df.to_csv('resolved-data/resolved-' + sim_file.strip())
            print("Done {}".format(sim_file))

    print(filename)


if __name__ == '__main__':
    # logging.basicConfig()
    # logging.getLogger().setLevel(logging.DEBUG)
    main()
